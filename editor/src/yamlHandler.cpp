#include "yamlHandler.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <climits>
#include <fstream>
#include <limits>

#include <yaml-cpp/yaml.h>

#include "mapElements.h"

bool YamlHandler::saveMapToYaml(const QString& fileName, const QList<MapElement*>& elements,
                                int terrainType, const QString& mapName) {
    try {
        YAML::Emitter out;
        out.SetIndent(2);  // Configurar indentación

        // Iniciar el documento YAML como un mapa
        out << YAML::BeginMap;

        // Nombre del mapa con comillas
        out << YAML::Key << "name" << YAML::Value << mapName.toStdString();

        // Determinar el tipo de mapa basado en el terrainType
        QString mapType;
        switch (terrainType) {
            case 0:  // DESERT_TERRAIN
                mapType = "Desert";
                break;
            case 1:  // AZTEC_VILLAGE
                mapType = "Aztec";
                break;
            case 2:  // TRAINING_GROUND
                mapType = "Training";
                break;
            default:
                mapType = "Desert";
                break;
        }

        // Agregar el campo map_type
        out << YAML::Key << "map_type" << YAML::Value << mapType.toStdString();

        // Separar los elementos por tipo (solo tiles y armas)
        QList<const Weapon*> weapons;
        QList<const Tile*> tiles;
        for (const MapElement* element: elements) {
            if (element->getType() == TILE) {
                tiles.append(static_cast<const Tile*>(element));
            } else if (element->getType() == WEAPON) {
                weapons.append(static_cast<const Weapon*>(element));
            }
        }

        // Guardar tiles
        out << YAML::Key << "tiles";
        out << YAML::Value << YAML::BeginSeq;

        // Crear un conjunto para evitar duplicados de IDs y rastrear los tiles realmente utilizados
        QSet<int> uniqueTileIds;

        // Primero determinamos qué IDs de tiles están realmente en uso en la matriz
        // Encontramos las dimensiones del mapa
        int maxX = 0;
        int maxY = 0;

        // Determinar el área real utilizada
        // Inicializar minX y minY con valores altos para encontrar el mínimo real
        int minX = INT_MAX;
        int minY = INT_MAX;
        for (const Tile* tile: tiles) {
            QPointF pos = tile->getPosition();
            int gridX = static_cast<int>(pos.x() / 32);
            int gridY = static_cast<int>(pos.y() / 32);

            // Actualizar min/max
            minX = std::min(minX, gridX);
            minY = std::min(minY, gridY);
            maxX = std::max(maxX, gridX);
            maxY = std::max(maxY, gridY);
        }

        // Crear una matriz para rastrear qué tiles se utilizan realmente
        std::vector<std::vector<int>> tileMatrix(
                maxY + 1, std::vector<int>(maxX + 1, 4));  // Default a 4 (solid)

        qDebug() << "Área real del mapa: " << minX << "," << minY << " a " << maxX << "," << maxY;

        // Calcular dimensiones de la matriz recortada
        int width = maxX - minX + 1;
        int height = maxY - minY + 1;

        // Redimensionar la matriz al tamaño exacto necesario
        tileMatrix.resize(height);
        for (auto& row: tileMatrix) {
            row.resize(width, 0);
        }

        // Llenar la matriz con los IDs de los tiles, ajustando posiciones al origen (minX, minY)
        for (const Tile* tile: tiles) {
            QPointF pos = tile->getPosition();
            int gridX = static_cast<int>(pos.x() / 32);
            int gridY = static_cast<int>(pos.y() / 32);
            int tileId = tile->getTileId();
            tileMatrix[gridY][gridX] = tileId;
            uniqueTileIds.insert(tileId);
        }

        // El ID 4 (solid) siempre se utiliza como valor predeterminado
        uniqueTileIds.insert(4);

        // Serializar cada tile único utilizado en la matriz (excluir el ID 0)
        for (int tileId: uniqueTileIds) {
            // Excluir explícitamente el ID 0 ya que no debe existir
            if (tileId == 0)
                continue;

            out << YAML::BeginMap;
            out << YAML::Key << "id" << YAML::Value << tileId;

            // Solo incluir el campo "type" para tiles especiales
            if (tileId == 1) {
                // Tile ID 1: Zona CT (antiterrorista)
                out << YAML::Key << "type" << YAML::Value << "ct_zone";
            } else if (tileId == 2) {
                // Tile ID 2: Zona T (terrorista)
                out << YAML::Key << "type" << YAML::Value << "t_zone";
            } else if (tileId == 3) {
                // Tile ID 3: Zona bomba
                out << YAML::Key << "type" << YAML::Value << "bomb_zone";
            } else if (tileId >= 4 && tileId <= 10) {
                // Tiles 4-10 son sólidos
                out << YAML::Key << "type" << YAML::Value << "solid";
            }
            // Para cualquier otro ID no incluimos el campo "type"

            out << YAML::EndMap;
        }
        out << YAML::EndSeq;

        // Guardar armas (array vacío si no hay)
        out << YAML::Key << "weapons";
        if (weapons.isEmpty()) {
            // Array vacío para armas (formato exacto solicitado)
            out << YAML::Value << "[]";
        } else {
            out << YAML::Value << YAML::BeginSeq;

            // Mapa de traducciones de IDs de armas a nombres de archivo
            // Estos son los nombres de archivo de las armas en ../client/assets/weapons/
            QMap<int, QString> weaponIdToName;
            // ID 1 = primera arma, ID 2 = segunda arma, etc.
            // Los ID números son asignados en loadAvailableWeapons() según el orden alfabético
            weaponIdToName[1] = "ak47";
            weaponIdToName[2] = "awp";
            weaponIdToName[3] = "carbine";
            weaponIdToName[4] = "pistol";
            weaponIdToName[5] = "rifle";
            weaponIdToName[6] = "shotgun";

            for (const Weapon* weapon: weapons) {
                out << YAML::BeginMap;

                // Obtener el tipo de arma como número (ID)
                int weaponId = weapon->getWeaponType();

                // Usar el nombre del archivo en lugar del ID numérico
                if (weaponIdToName.contains(weaponId)) {
                    out << YAML::Key << "type" << YAML::Value
                        << weaponIdToName[weaponId].toStdString();
                } else {
                    // Si no encontramos el nombre, usamos el ID como respaldo
                    qWarning() << "No se encontró nombre para el arma con ID:" << weaponId;
                    out << YAML::Key << "type" << YAML::Value << weaponId;
                }

                out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq
                    << weapon->getPosition().x() << weapon->getPosition().y() << YAML::EndSeq;
                out << YAML::EndMap;
            }
            out << YAML::EndSeq;
        }

        // Generar la matriz de tiles para el formato requerido
        // Ya tenemos los datos en uniqueTileIds y el array tileMatrix desde antes
        out << YAML::Key << "matrix";
        out << YAML::Value << YAML::BeginSeq;
        for (const auto& row: tileMatrix) {
            out << YAML::Flow << row;
        }
        out << YAML::EndSeq;

        // Terminar el documento YAML
        out << YAML::EndMap;

        // Escribir a archivo
        std::ofstream fout(fileName.toStdString());
        if (!fout.is_open()) {
            qDebug() << "No se pudo abrir el archivo para escritura:" << fileName;
            return false;
        }

        // Escribir el contenido sin separadores adicionales "---"
        fout << out.c_str();
        fout.close();

        qDebug() << "Archivo guardado con éxito:" << fileName;
        return true;
    } catch (const YAML::Exception& e) {
        qDebug() << "Error al guardar YAML:" << e.what();
        return false;
    }
}

bool YamlHandler::loadMapFromYaml(const QString& fileName, QList<MapElement*>& elements,
                                  int& terrainType, QString& mapName) {
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
                terrainType = 0;  // DESERT_TERRAIN por defecto
            }
        } else if (mainNode["terrain_type"]) {
            // Si hay terrain_type directo (formato antiguo)
            terrainType = mainNode["terrain_type"].as<int>();
        } else {
            terrainType = 0;  // DESERT_TERRAIN por defecto
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
            for (const auto& weaponNode: weaponsNode) {
                if (weaponNode["type"] && weaponNode["position"]) {
                    int weaponType = weaponNode["type"].as<int>();
                    std::vector<double> pos = weaponNode["position"].as<std::vector<double>>();

                    if (pos.size() >= 2) {
                        QPointF position(pos[0], pos[1]);
                        Weapon* weapon = new Weapon(position, weaponType);
                        elements.append(weapon);
                        qDebug() << "Cargada arma tipo:" << weaponType << "en posición:" << pos[0]
                                 << "," << pos[1];
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
                            if (tileId > 0) {                      // Ignorar celdas vacías (0)
                                QPointF position(x * 32, y * 32);  // Usar grilla de 32x32
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
                for (const auto& weaponNode: weaponsNode) {
                    Weapon* weapon = deserializeWeapon(weaponNode);
                    if (weapon) {
                        elements.append(weapon);
                    }
                }
            }

            // Intentar cargar tiles
            if (mainNode["tiles"] && mainNode["tiles"].IsSequence()) {
                YAML::Node tilesNode = mainNode["tiles"];
                for (const auto& tileNode: tilesNode) {
                    Tile* tile = deserializeTile(tileNode);
                    if (tile) {
                        elements.append(tile);
                    }
                }
            }

            qDebug() << "Cargados" << elements.size() << "elementos usando formato antiguo";
        }

        return true;
    } catch (const YAML::Exception& e) {
        qDebug() << "Error al cargar YAML:" << e.what();
        return false;
    }
}


void YamlHandler::serializeWeapon(YAML::Emitter& out, const Weapon* weapon) {
    out << YAML::BeginMap;
    // Formato exacto como en el ejemplo
    out << YAML::Key << "type" << YAML::Value << weapon->getWeaponType();
    // Posición formateada como array [x, y]
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq
        << weapon->getPosition().x() << weapon->getPosition().y() << YAML::EndSeq;
    out << YAML::EndMap;
}

void YamlHandler::serializeTile(YAML::Emitter& out, const Tile* tile) {
    out << YAML::BeginMap;

    // ID del tile (en el formato requerido)
    out << YAML::Key << "id" << YAML::Value << tile->getTileId();

    // Propiedades del tile según su ID
    int tileId = tile->getTileId();

    // Tipo de tile (derivado del ID)
    QString tileType;

    if (tileId == 1) {
        // Tile ID 1: Zona CT (antiterrorista)
        tileType = "ct_zone";
    } else if (tileId == 2) {
        // Tile ID 2: Zona T (terrorista)
        tileType = "t_zone";
    } else if (tileId == 3) {
        // Tile ID 3: Zona bomba
        tileType = "bomb_zone";
    } else if (tileId >= 4 && tileId <= 10) {
        // Tiles 4-10 son sólidos
        tileType = "solid";
    } else {
        // Los demás tiles son transitables (walkable)
        tileType = "walkable";
    }

    // Tipo (en formato de comillas como en el ejemplo)
    out << YAML::Key << "type" << YAML::Value << "\"" + tileType.toStdString() + "\"";

    out << YAML::EndMap;
}


Weapon* YamlHandler::deserializeWeapon(const YAML::Node& node) {
    if (!node["type"] || !node["position"]) {
        return nullptr;
    }

    int type = node["type"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());

    return new Weapon(pos, type);
}

Tile* YamlHandler::deserializeTile(const YAML::Node& node) {
    if (!node["tile_id"] || !node["position"]) {
        return nullptr;
    }

    int tileId = node["tile_id"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());

    // Construir el Tile con su ID y posición
    Tile* tile = new Tile(pos, tileId);

    // Comprobar si hay información de tipo en el nodo
    if (node["type"]) {
        std::string typeStr = node["type"].as<std::string>();
        qDebug() << "Tipo de tile cargado:" << QString::fromStdString(typeStr)
                 << "para ID:" << tileId;
    }

    // Comprobar si hay información de solidez
    if (node["solid"]) {
        bool isSolid = node["solid"].as<bool>();
        qDebug() << "Propiedad sólido:" << isSolid << "para tile ID:" << tileId;
    }

    // Comprobar si hay información de zona de bomba
    if (node["bomb_zone"]) {
        bool isBombZone = node["bomb_zone"].as<bool>();
        qDebug() << "Propiedad zona bomba:" << isBombZone << "para tile ID:" << tileId;
    }

    return tile;
}
