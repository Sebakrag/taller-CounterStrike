#include "yamlHandler.h"
#include <QFile>
#include <QDebug>
#include <fstream>

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
        out << YAML::Key << "terrain_type" << YAML::Value << terrainType;
        
        // Tamaño del mapa (por defecto)
        out << YAML::Key << "size";
        out << YAML::Value << YAML::BeginMap;
        out << YAML::Key << "width" << YAML::Value << 1024;
        out << YAML::Key << "height" << YAML::Value << 1024;
        out << YAML::EndMap;
        
        // Zonas de inicio de equipos
        out << YAML::Key << "team_spawns";
        out << YAML::Value << YAML::BeginSeq;
        
        // Zonas de bombas
        out << YAML::Key << "bomb_zones";
        out << YAML::Value << YAML::BeginSeq;
        
        // Estructuras sólidas
        out << YAML::Key << "solid_structures";
        out << YAML::Value << YAML::BeginSeq;
        
        // Armas
        out << YAML::Key << "weapons";
        out << YAML::Value << YAML::BeginSeq;
        
        // Procesar cada elemento según su tipo
        for (const MapElement* element : elements) {
            switch (element->getType()) {
                case TEAM_SPAWN_CT:
                case TEAM_SPAWN_T:
                    serializeTeamSpawn(out, static_cast<const TeamSpawn*>(element));
                    break;
                case BOMB_ZONE:
                    serializeBombZone(out, static_cast<const BombZone*>(element));
                    break;
                case SOLID_STRUCTURE:
                    serializeSolidStructure(out, static_cast<const SolidStructure*>(element));
                    break;
                case WEAPON:
                    serializeWeapon(out, static_cast<const Weapon*>(element));
                    break;
            }
        }
        
        // Cerrar secuencias
        out << YAML::EndSeq; // weapons
        out << YAML::EndSeq; // solid_structures
        out << YAML::EndSeq; // bomb_zones
        out << YAML::EndSeq; // team_spawns
        
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
        
        // Leer zonas de inicio de equipos
        if (mapNode["team_spawns"]) {
            YAML::Node teamSpawnsNode = mapNode["team_spawns"];
            for (const auto& spawnNode : teamSpawnsNode) {
                TeamSpawn* spawn = deserializeTeamSpawn(spawnNode);
                if (spawn) {
                    elements.append(spawn);
                }
            }
        }
        
        // Leer zonas de bombas
        if (mapNode["bomb_zones"]) {
            YAML::Node bombZonesNode = mapNode["bomb_zones"];
            for (const auto& zoneNode : bombZonesNode) {
                BombZone* zone = deserializeBombZone(zoneNode);
                if (zone) {
                    elements.append(zone);
                }
            }
        }
        
        // Leer estructuras sólidas
        if (mapNode["solid_structures"]) {
            YAML::Node structuresNode = mapNode["solid_structures"];
            for (const auto& structNode : structuresNode) {
                SolidStructure* structure = deserializeSolidStructure(structNode);
                if (structure) {
                    elements.append(structure);
                }
            }
        }
        
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
        
        return true;
    } catch (const YAML::Exception &e) {
        qDebug() << "Error al cargar YAML:" << e.what();
        return false;
    }
}

// Métodos de serialización
void YamlHandler::serializeTeamSpawn(YAML::Emitter &out, const TeamSpawn *spawn) {
    out << YAML::BeginMap;
    out << YAML::Key << "team_id" << YAML::Value << spawn->getTeamId();
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << spawn->getPosition().x() << spawn->getPosition().y() << YAML::EndSeq;
    out << YAML::EndMap;
}

void YamlHandler::serializeBombZone(YAML::Emitter &out, const BombZone *zone) {
    out << YAML::BeginMap;
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << zone->getPosition().x() << zone->getPosition().y() << YAML::EndSeq;
    out << YAML::Key << "size" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << zone->getSize().width() << zone->getSize().height() << YAML::EndSeq;
    out << YAML::EndMap;
}

void YamlHandler::serializeSolidStructure(YAML::Emitter &out, const SolidStructure *structure) {
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << structure->getStructureType();
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << structure->getPosition().x() << structure->getPosition().y() << YAML::EndSeq;
    out << YAML::EndMap;
}

void YamlHandler::serializeWeapon(YAML::Emitter &out, const Weapon *weapon) {
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << weapon->getWeaponType();
    out << YAML::Key << "position" << YAML::Value << YAML::Flow << YAML::BeginSeq 
        << weapon->getPosition().x() << weapon->getPosition().y() << YAML::EndSeq;
    out << YAML::EndMap;
}

// Métodos de deserialización
TeamSpawn* YamlHandler::deserializeTeamSpawn(const YAML::Node &node) {
    if (!node["team_id"] || !node["position"]) {
        return nullptr;
    }
    
    int teamId = node["team_id"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
    
    return new TeamSpawn(pos, teamId);
}

BombZone* YamlHandler::deserializeBombZone(const YAML::Node &node) {
    if (!node["position"]) {
        return nullptr;
    }
    
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
    
    QSizeF size(50, 50); // Tamaño por defecto
    if (node["size"]) {
        auto sizeNode = node["size"];
        size = QSizeF(sizeNode[0].as<float>(), sizeNode[1].as<float>());
    }
    
    return new BombZone(pos, size);
}

SolidStructure* YamlHandler::deserializeSolidStructure(const YAML::Node &node) {
    if (!node["type"] || !node["position"]) {
        return nullptr;
    }
    
    int type = node["type"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
    
    return new SolidStructure(pos, type);
}

Weapon* YamlHandler::deserializeWeapon(const YAML::Node &node) {
    if (!node["type"] || !node["position"]) {
        return nullptr;
    }
    
    int type = node["type"].as<int>();
    auto posNode = node["position"];
    QPointF pos(posNode[0].as<float>(), posNode[1].as<float>());
    
    return new Weapon(pos, type);
}
