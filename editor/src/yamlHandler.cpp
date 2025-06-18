#include "yamlHandler.h"
#include <QFile>
#include <QDebug>
#include <fstream>
#include "yamlHandler.h"
#include "mapElements.h"

bool YamlHandler::saveMapToYaml(const QString &fileName, 
                                const QList<MapElement*> &elements, 
                                int terrainType,
                                const QString &mapName) {
    try {
        YAML::Emitter out;
        
        // Determinar el tipo de mapa basado en el terrainType
        QString mapType;
        switch (terrainType) {
            case 0: // DESERT_TERRAIN
                mapType = "Desert";
                break;
            case 1: // AZTEC_VILLAGE
                mapType = "Aztec";
                break;
            case 2: // TRAINING_GROUND
                mapType = "Training";
                break;
            default:
                mapType = "Desert";
                break;
        }
        
        // Identificar todos los tiles
        QList<const Tile*> tiles;
        for (const MapElement* element : elements) {
            if (element->getType() == TILE) {
                tiles.append(static_cast<const Tile*>(element));
            }
        }
        
        if (tiles.isEmpty()) {
            qDebug() << "No hay tiles para guardar en el mapa";
            return false;
        }
        
        // Encontrar dimensiones reales del mapa utilizadas (min/max)
        int minX = INT_MAX;
        int minY = INT_MAX;
        int maxX = 0;
        int maxY = 0;
        
        // Determinar el área real utilizada
        for (const Tile* tile : tiles) {
            QPointF pos = tile->getPosition();
            int gridX = static_cast<int>(pos.x() / 32);
            int gridY = static_cast<int>(pos.y() / 32);
            
            // Actualizar min/max
            minX = std::min(minX, gridX);
            minY = std::min(minY, gridY);
            maxX = std::max(maxX, gridX);
            maxY = std::max(maxY, gridY);
        }
        
        // Asegurarse de que tenemos dimensiones válidas
        if (minX > maxX || minY > maxY) {
            qDebug() << "Error al determinar dimensiones del mapa";
            return false;
        }
        
        qDebug() << "Área real del mapa: " << minX << "," << minY << " a " << maxX << "," << maxY;
        
        // Calcular dimensiones de la matriz recortada
        int width = maxX - minX + 1;
        int height = maxY - minY + 1;
        
        // Crear una matriz vacía del tamaño exacto necesario
        std::vector<std::vector<int>> tileMatrix(height, std::vector<int>(width, 0));
        
        // Llenar la matriz con los IDs de los tiles, ajustando posiciones al origen (minX, minY)
        for (const Tile* tile : tiles) {
            QPointF pos = tile->getPosition();
            int gridX = static_cast<int>(pos.x() / 32) - minX;
            int gridY = static_cast<int>(pos.y() / 32) - minY;
            
            // Estas coordenadas deberían estar siempre dentro de los límites
            if (gridX >= 0 && gridX < width && gridY >= 0 && gridY < height) {
                tileMatrix[gridY][gridX] = tile->getTileId();
            }
        }
        
        // Formato YAML simplificado con nombre, map_type y matrix
        out << YAML::BeginMap;
        
        // Incluir el nombre del mapa
        out << YAML::Key << "name" << YAML::Value << mapName.toStdString();
        
        // Incluir map_type
        out << YAML::Key << "map_type" << YAML::Value << mapType.toStdString();
        
        // Guardar la matriz de tiles
        out << YAML::Key << "matrix";
        out << YAML::Value << YAML::BeginSeq;
        for (const auto& row : tileMatrix) {
            out << YAML::Flow << row;
        }
        out << YAML::EndSeq;
        
        out << YAML::EndMap;
        
        // Escribir el YAML al archivo
        std::ofstream fout(fileName.toStdString().c_str());
        if (!fout) {
            qDebug() << "No se puede abrir el archivo para escritura:" << fileName;
            return false;
        }
        
        fout << out.c_str();
        fout.close();
        
        qDebug() << "Formato YAML simplificado guardado con éxito con SOLO map_type y matrix";
        return true;
    } catch (const YAML::Exception &e) {
        qDebug() << "Error al guardar YAML:" << e.what();
        return false;
    }
}

bool YamlHandler::loadMapFromYaml(const QString &fileName, 
                                 QList<MapElement*> &elements, 
                                 int &terrainType,
                                 QString &mapName) {
    try {
        // Limpiar la lista de elementos existentes
        qDeleteAll(elements);
        elements.clear();
        
        // Cargar el archivo YAML
        YAML::Node rootNode = YAML::LoadFile(fileName.toStdString());
        
        if (!rootNode["map"]) {
            qDebug() << "Formato de archivo inválido: no se encontró el nodo 'map'";
            return false;
        }
        
        YAML::Node mapNode = rootNode["map"];
        
        // Leer información básica del mapa
        if (mapNode["name"]) {
            mapName = QString::fromStdString(mapNode["name"].as<std::string>());
        }
        
        if (mapNode["terrain_type"]) {
            terrainType = mapNode["terrain_type"].as<int>();
        }
        
        // Se eliminaron las secciones de carga de team_spawns, bomb_zones y solid_structures
        // Solo cargaremos tiles y armas
        
        // Leer armas
        if (mapNode["weapons"]) {
            YAML::Node weaponsNode = mapNode["weapons"];
            for (const auto& weaponNode : weaponsNode) {
                Weapon* weapon = deserializeWeapon(weaponNode);
                if (weapon) {
                    elements.append(weapon);
                }
            }
        }
        
        // Leer tiles
        if (mapNode["tiles"]) {
            YAML::Node tilesNode = mapNode["tiles"];
            for (const auto& tileNode : tilesNode) {
                Tile* tile = deserializeTile(tileNode);
                if (tile) {
                    elements.append(tile);
                    qDebug() << "Cargado tile desde archivo:" << tile->getTileId() << "en posición" 
                            << tile->getPosition().x() << "," << tile->getPosition().y();
                }
            }
        }
        
        // Para compatibilidad con archivos antiguos que puedan tener extra_tiles
        if (mapNode["extra_tiles"]) {
            YAML::Node extraTilesNode = mapNode["extra_tiles"];
            for (const auto& extraTileNode : extraTilesNode) {
                if (extraTileNode["extra_tile_id"] && extraTileNode["position"]) {
                    int tileId = extraTileNode["extra_tile_id"].as<int>();
                    auto posNode = extraTileNode["position"];
                    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
                    
                    // Crear un tile normal con el ID del extra tile
                    Tile* tile = new Tile(pos, tileId);
                    elements.append(tile);
                    qDebug() << "Cargado extra tile (como tile normal) desde archivo:" << tileId << "en posición" 
                            << pos.x() << "," << pos.y();
                }
            }
        }
        
        // Verificar si existe la matriz de tiles para compatibilidad con la aplicación
        // Esto es solo para información, ya que los tiles individuales ya se cargaron arriba
        if (mapNode["tile_matrix"]) {
            YAML::Node matrixNode = mapNode["tile_matrix"];
            qDebug() << "Encontrada matriz de tiles para compatibilidad con la aplicación";
            qDebug() << "Dimensiones de la matriz: " << matrixNode.size() << " filas";
            if (matrixNode.size() > 0) {
                qDebug() << "Primera fila tiene " << matrixNode[0].size() << " columnas";
            }
        }
        
        // Verificar si existe la información adicional para la aplicación
        if (mapNode["application_info"]) {
            YAML::Node appInfoNode = mapNode["application_info"];
            qDebug() << "Encontrada información adicional para la aplicación";
            
            if (appInfoNode["map_type"]) {
                std::string mapTypeStr = appInfoNode["map_type"].as<std::string>();
                qDebug() << "Tipo de mapa: " << QString::fromStdString(mapTypeStr);
                
                // Actualizar el terrainType basado en el map_type
                if (mapTypeStr == "Desert") terrainType = 0;
                else if (mapTypeStr == "Aztec") terrainType = 1;
                else if (mapTypeStr == "Training") terrainType = 2;
            }
            
            if (appInfoNode["width"] && appInfoNode["height"]) {
                int width = appInfoNode["width"].as<int>();
                int height = appInfoNode["height"].as<int>();
                qDebug() << "Dimensiones del mapa: " << width << "x" << height;
            }
        }
        
        return true;
    } catch (const YAML::Exception &e) {
        qDebug() << "Error al cargar YAML:" << e.what();
        return false;
    }
}

// Se eliminaron los métodos de serialización para TeamSpawn, BombZone y SolidStructure

void YamlHandler::serializeWeapon(YAML::Emitter &out, const Weapon *weapon) {
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << weapon->getWeaponType();
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << weapon->getPosition().x() << weapon->getPosition().y() << YAML::EndSeq;
    out << YAML::EndMap;
}

void YamlHandler::serializeTile(YAML::Emitter &out, const Tile *tile) {
    out << YAML::BeginMap;
    out << YAML::Key << "tile_id" << YAML::Value << tile->getTileId();
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << tile->getPosition().x() << tile->getPosition().y() << YAML::EndSeq;
    out << YAML::EndMap;
}

// Se eliminaron los métodos de deserialización para TeamSpawn, BombZone y SolidStructure

Weapon* YamlHandler::deserializeWeapon(const YAML::Node &node) {
    if (!node["type"] || !node["position"]) {
        return nullptr;
    }
    
    int type = node["type"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
    
    return new Weapon(pos, type);
}

Tile* YamlHandler::deserializeTile(const YAML::Node &node) {
    if (!node["tile_id"] || !node["position"]) {
        return nullptr;
    }
    
    int tileId = node["tile_id"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
    
    return new Tile(pos, tileId);
}

// Los métodos para ExtraTile han sido eliminados ya que los extra tiles se manejan como tiles normales
