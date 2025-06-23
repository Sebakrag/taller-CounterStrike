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
    
private:
};
