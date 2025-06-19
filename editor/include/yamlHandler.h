#pragma once

#include <QString>
#include <QList>
#include <yaml-cpp/yaml.h>
#include "mapElements.h"

class YamlHandler {
public:
    // Guardar mapa en formato YAML
    static bool saveMapToYaml(const QString &fileName, 
                             const QList<MapElement*> &elements, 
                             int terrainType,
                             const QString &mapName);
    
    // Cargar mapa desde archivo YAML
    static bool loadMapFromYaml(const QString &fileName, 
                               QList<MapElement*> &elements, 
                               int &terrainType,
                               QString &mapName);
    
private:
    // Métodos auxiliares para serialización
    static void serializeTeamSpawn(YAML::Emitter &out, const TeamSpawn *spawn);
    static void serializeBombZone(YAML::Emitter &out, const BombZone *zone);
    static void serializeSolidStructure(YAML::Emitter &out, const SolidStructure *structure);
    static void serializeWeapon(YAML::Emitter &out, const Weapon *weapon);
    static void serializeTile(YAML::Emitter &out, const Tile *tile);
    
    // Métodos auxiliares para deserialización
    static TeamSpawn* deserializeTeamSpawn(const YAML::Node &node);
    static BombZone* deserializeBombZone(const YAML::Node &node);
    static SolidStructure* deserializeSolidStructure(const YAML::Node &node);
    static Weapon* deserializeWeapon(const YAML::Node &node);
    static Tile* deserializeTile(const YAML::Node &node);
};
