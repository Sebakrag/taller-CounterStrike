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
        
        // Inicio del documento YAML
        out << YAML::BeginMap;
        
        // Información básica del mapa
        out << YAML::Key << "map";
        out << YAML::Value << YAML::BeginMap;
        
        out << YAML::Key << "name" << YAML::Value << mapName.toStdString();
        
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
        
        // Agregar el nuevo campo map_type
        out << YAML::Key << "map_type" << YAML::Value << mapType.toStdString();
        // Mantener terrain_type por compatibilidad
        out << YAML::Key << "terrain_type" << YAML::Value << terrainType;
        
        // Tamaño del mapa (por defecto)
        out << YAML::Key << "size";
        out << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "width" << YAML::Value << 1024;
        out << YAML::Key << "height" << YAML::Value << 1024;
        out << YAML::EndMap;
        
        // Separar los elementos por tipo (solo tiles y armas)
        QList<const Weapon*> weapons;
        QList<const Tile*> tiles;
        
        // Clasificar los elementos según su tipo
        for (const MapElement* element : elements) {
            switch (element->getType()) {
                case WEAPON:
                    weapons.append(static_cast<const Weapon*>(element));
                    break;
                case TILE:
                    tiles.append(static_cast<const Tile*>(element));
                    break;
                default:
                    // Ignorar otros tipos de elementos
                    break;
            }
        }
        
        // Se eliminaron las secciones de team_spawns, bomb_zones y solid_structures
        
        // Guardar armas
        out << YAML::Key << "weapons";
        out << YAML::Value << YAML::BeginSeq;
        for (const Weapon* weapon : weapons) {
            serializeWeapon(out, weapon);
        }
        out << YAML::EndSeq;
        
        // Guardar tiles
        out << YAML::Key << "tiles";
        out << YAML::Value << YAML::BeginSeq;
        for (const Tile* tile : tiles) {
            serializeTile(out, tile);
        }
        out << YAML::EndSeq;
        
        // Los extra tiles se manejan como tiles normales, ya están incluidos en la sección de tiles
        
        // Generar una matriz de tiles para compatibilidad con la aplicación
        // Primero encontramos las dimensiones del mapa
        int maxX = 0;
        int maxY = 0;
        for (const Tile* tile : tiles) {
            QPointF pos = tile->getPosition();
            // Convertir posición a coordenadas de grilla (asumiendo una grilla de 32x32)
            int gridX = static_cast<int>(pos.x() / 32);
            int gridY = static_cast<int>(pos.y() / 32);
            if (gridX > maxX) maxX = gridX;
            if (gridY > maxY) maxY = gridY;
        }
        
        // Crear una matriz vacía con un valor predeterminado (0 = sin tile)
        std::vector<std::vector<int>> tileMatrix(maxY + 1, std::vector<int>(maxX + 1, 0));
        
        // Llenar la matriz con los IDs de los tiles
        for (const Tile* tile : tiles) {
            QPointF pos = tile->getPosition();
            // Convertir posición a coordenadas de grilla (asumiendo una grilla de 32x32)
            int gridX = static_cast<int>(pos.x() / 32);
            int gridY = static_cast<int>(pos.y() / 32);
            tileMatrix[gridY][gridX] = tile->getTileId();
        }
        
        // Guardar la matriz de tiles para compatibilidad con la aplicación
        out << YAML::Key << "tile_matrix";
        out << YAML::Value << YAML::BeginSeq;
        for (const auto& row : tileMatrix) {
            out << YAML::Flow << row;
        }
        out << YAML::EndSeq;
        
        // Añadir información adicional para compatibilidad con la aplicación
        out << YAML::Key << "application_info";
        out << YAML::Value << YAML::BeginMap;
        
        // Tipo de mapa (Desert, Aztec, Training)
        std::string mapTypeStr;
        switch (terrainType) {
            case 0: mapTypeStr = "Desert"; break;
            case 1: mapTypeStr = "Aztec"; break;
            case 2: mapTypeStr = "Training"; break;
            default: mapTypeStr = "Desert"; break;
        }
        out << YAML::Key << "map_type" << YAML::Value << mapTypeStr;
        
        // Dimensiones
        out << YAML::Key << "width" << YAML::Value << (maxX + 1);
        out << YAML::Key << "height" << YAML::Value << (maxY + 1);
        
        out << YAML::EndMap;
        
        // Cerrar mapa
        out << YAML::EndMap; // map
        out << YAML::EndMap; // documento
        
        // Escribir a archivo
        std::ofstream fout(fileName.toStdString());
        if (!fout.is_open()) {
            qDebug() << "No se pudo abrir el archivo para escritura:" << fileName;
            return false;
        }
        
        fout << out.c_str();
        fout.close();
        
        return true;
    } catch (const YAML::Exception &e) {
        qDebug() << "Error al generar YAML:" << e.what();
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
