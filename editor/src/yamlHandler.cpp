#include "yamlHandler.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <yaml-cpp/yaml.h>
#include <limits>
#include <climits>
#include <fstream>
#include "mapElements.h"

bool YamlHandler::saveMapToYaml(const QString &fileName, 
                                const QList<MapElement*> &elements, 
                                int terrainType,
                                const QString &mapName) {
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
        
        // Agregar el campo map_type
        out << YAML::Key << "map_type" << YAML::Value << mapType.toStdString();
        
        // Separar los elementos por tipo (solo tiles y armas)
        QList<const Weapon*> weapons;
        QList<const Tile*> tiles;
        for (const MapElement* element : elements) {
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
        
        // Crear una matriz para rastrear qué tiles se utilizan realmente
        std::vector<std::vector<int>> tileMatrix(maxY + 1, std::vector<int>(maxX + 1, 4)); // Default a 4 (solid)
        
        qDebug() << "Área real del mapa: " << minX << "," << minY << " a " << maxX << "," << maxY;
        
        // Calcular dimensiones de la matriz recortada
        int width = maxX - minX + 1;
        int height = maxY - minY + 1;
        
        // Redimensionar la matriz al tamaño exacto necesario
        tileMatrix.resize(height);
        for (auto& row : tileMatrix) {
            row.resize(width, 0);
        }
        
        // Llenar la matriz con los IDs de los tiles, ajustando posiciones al origen (minX, minY)
        for (const Tile* tile : tiles) {
            QPointF pos = tile->getPosition();
            int gridX = static_cast<int>(pos.x() / 32);
            int gridY = static_cast<int>(pos.y() / 32);
            int tileId = tile->getTileId();
            tileMatrix[gridY][gridX] = tileId;
            uniqueTileIds.insert(tileId);
        }
        
        // Serializar cada tile único utilizado en la matriz (excluir el ID 0)
        for (int tileId : uniqueTileIds) {
            // Excluir explícitamente el ID 0 ya que no debe existir
            if (tileId == 0) continue;
            
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
            } else if (tileId >= 4 && tileId <= 24) {
                // Tiles 4-24 son sólidos
                out << YAML::Key << "type" << YAML::Value << "solid";
            }
            
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
            // Estos son los nombres de archivo de las armas en client/assets/gfx/weapons/
            QMap<int, QString> weaponIdToName;
            // ID 1 = primera arma, ID 2 = segunda arma, etc.
            // Los ID números son asignados en loadAvailableWeapons() según el orden alfabético
            weaponIdToName[1] = "ak47";
            weaponIdToName[2] = "awp";
            weaponIdToName[3] = "glock";
            weaponIdToName[4] = "m3";
            
            for (const Weapon* weapon : weapons) {
                out << YAML::BeginMap;
                
                // Obtener el tipo de arma como número (ID)
                int weaponId = weapon->getWeaponType();
                
                // Usar el nombre del archivo en lugar del ID numérico
                if (weaponIdToName.contains(weaponId)) {
                    out << YAML::Key << "type" << YAML::Value << weaponIdToName[weaponId].toStdString();
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
        for (const auto& row : tileMatrix) {
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
    } catch (const YAML::Exception &e) {
        qDebug() << "Error al guardar YAML:" << e.what();
        return false;
    }
}

