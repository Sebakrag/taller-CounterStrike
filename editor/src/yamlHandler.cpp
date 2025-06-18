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
        
        // Identificar todos los tiles y armas
        QList<const Tile*> tiles;
        QList<const Weapon*> weapons;
        for (const MapElement* element : elements) {
            if (element->getType() == TILE) {
                tiles.append(static_cast<const Tile*>(element));
            } else if (element->getType() == WEAPON) {
                weapons.append(static_cast<const Weapon*>(element));
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
        
        // Guardar las armas si existen
        if (!weapons.isEmpty()) {
            out << YAML::Key << "weapons";
            out << YAML::Value << YAML::BeginSeq;
            
            for (const Weapon* weapon : weapons) {
                out << YAML::BeginMap;
                
                // Guardar el tipo de arma
                out << YAML::Key << "type" << YAML::Value << weapon->getWeaponType();
                
                // Guardar la posición
                QPointF pos = weapon->getPosition();
                out << YAML::Key << "position";
                out << YAML::Value << YAML::Flow << std::vector<double>{pos.x(), pos.y()};
                
                out << YAML::EndMap;
            }
            
            out << YAML::EndSeq;
            qDebug() << "Guardadas" << weapons.size() << "armas en el YAML";
        }
        
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
        
        // Abrir archivo YAML
        YAML::Node rootNode = YAML::LoadFile(fileName.toStdString());
        
        // Verificar si es el nuevo formato simplificado o el antiguo
        bool isNewFormat = rootNode["name"] && rootNode["map_type"] && rootNode["matrix"];
        bool isOldFormat = rootNode["map"] && rootNode["map"]["name"];
        
        if (!isNewFormat && !isOldFormat) {
            qDebug() << "Error: Formato de archivo YAML no reconocido";
            return false;
        }
        
        // Referencia al nodo principal (puede ser la raíz o el nodo 'map')
        YAML::Node mainNode;
        
        if (isNewFormat) {
            mainNode = rootNode;
            qDebug() << "Cargando mapa con formato YAML simplificado";
        } else {
            mainNode = rootNode["map"];
            qDebug() << "Cargando mapa con formato YAML antiguo";
        }
        
        // Leer nombre del mapa
        if (mainNode["name"]) {
            mapName = QString::fromStdString(mainNode["name"].as<std::string>());
        } else {
            mapName = "Mapa sin nombre";
        }
        
        // Determinar el tipo de terreno
        if (mainNode["map_type"]) {
            // Convertir map_type string a terrainType int
            std::string mapTypeStr = mainNode["map_type"].as<std::string>();
            if (mapTypeStr == "Desert") {
                terrainType = 0;
            } else if (mapTypeStr == "Aztec") {
                terrainType = 1;
            } else if (mapTypeStr == "Training") {
                terrainType = 2;
            } else {
                terrainType = 0; // DESERT_TERRAIN por defecto
            }
        } else if (mainNode["terrain_type"]) {
            // Si hay terrain_type directo (formato antiguo)
            terrainType = mainNode["terrain_type"].as<int>();
        } else {
            terrainType = 0; // DESERT_TERRAIN por defecto
        }
        
        // Verificar si es el nuevo formato con matrix o el antiguo con tile_matrix
        YAML::Node matrixNode;
        
        if (mainNode["matrix"] && mainNode["matrix"].IsSequence()) {
            matrixNode = mainNode["matrix"];
        } else if (mainNode["tile_matrix"] && mainNode["tile_matrix"].IsSequence()) {
            matrixNode = mainNode["tile_matrix"];
        }
        
        // Cargar las armas (formato nuevo)
        if (mainNode["weapons"] && mainNode["weapons"].IsSequence()) {
            YAML::Node weaponsNode = mainNode["weapons"];
            for (const auto& weaponNode : weaponsNode) {
                if (weaponNode["type"] && weaponNode["position"]) {
                    int weaponType = weaponNode["type"].as<int>();
                    std::vector<double> pos = weaponNode["position"].as<std::vector<double>>();
                    
                    if (pos.size() >= 2) {
                        QPointF position(pos[0], pos[1]);
                        Weapon* weapon = new Weapon(position, weaponType);
                        elements.append(weapon);
                        qDebug() << "Cargada arma tipo:" << weaponType << "en posición:" << pos[0] << "," << pos[1];
                    }
                }
            }
        }
        
        // Cargar matriz de tiles
        if (matrixNode) {
            int rows = matrixNode.size();
            
            if (rows > 0) {
                for (int y = 0; y < rows; y++) {
                    if (matrixNode[y].IsSequence()) {
                        YAML::Node row = matrixNode[y];
                        int cols = row.size();
                        
                        for (int x = 0; x < cols; x++) {
                            int tileId = row[x].as<int>();
                            if (tileId > 0) { // Ignorar celdas vacías (0)
                                QPointF position(x * 32, y * 32); // Usar grilla de 32x32
                                Tile* tile = new Tile(position, tileId);
                                elements.append(tile);
                                
                                if (tileId <= 3) {
                                    qDebug() << "Cargado tile especial:" << tileId << "en posición" 
                                            << x * 32 << "," << y * 32;
                                }
                            }
                        }
                    }
                }
                
                qDebug() << "Cargados" << elements.size() << "elementos desde la matriz";
            }
        }
        // Si no hay matrix ni tile_matrix, intentar con el formato antiguo
        else if (isOldFormat) {
            // Intentar cargar armas
            if (mainNode["weapons"] && mainNode["weapons"].IsSequence()) {
                YAML::Node weaponsNode = mainNode["weapons"];
                for (const auto& weaponNode : weaponsNode) {
                    Weapon* weapon = deserializeWeapon(weaponNode);
                    if (weapon) {
                        elements.append(weapon);
                    }
                }
            }
            
            // Intentar cargar tiles
            if (mainNode["tiles"] && mainNode["tiles"].IsSequence()) {
                YAML::Node tilesNode = mainNode["tiles"];
                for (const auto& tileNode : tilesNode) {
                    Tile* tile = deserializeTile(tileNode);
                    if (tile) {
                        elements.append(tile);
                    }
                }
            }
            
            qDebug() << "Cargados" << elements.size() << "elementos usando formato antiguo";
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
