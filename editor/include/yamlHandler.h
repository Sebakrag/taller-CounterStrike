#pragma once

#include <QList>
#include <QString>

#include <yaml-cpp/yaml.h>

#include "mapElements.h"

class YamlHandler {
public:
    // Guardar mapa en formato YAML
    static bool saveMapToYaml(const QString& fileName, const QList<MapElement*>& elements,
                              int terrainType, const QString& mapName);

    // Cargar mapa desde archivo YAML
    static bool loadMapFromYaml(const QString& fileName, QList<MapElement*>& elements,
                                int& terrainType, QString& mapName);

private:
    // Métodos auxiliares para serialización
    static void serializeWeapon(YAML::Emitter& out, const Weapon* weapon);
    static void serializeTile(YAML::Emitter& out, const Tile* tile);

    // Métodos auxiliares para deserialización
    static Weapon* deserializeWeapon(const YAML::Node& node);
    static Tile* deserializeTile(const YAML::Node& node);
};
