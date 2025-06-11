#ifndef SCENARIO_REGISTRY_H_
#define SCENARIO_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "../../common/dtos/WindowConfig.h"
#include "../../common/tile_map.h"

class ScenarioRegistry {
private:
    static std::map<std::string, TileMap> scenarios;
    static WindowConfig windowConfig;
    static bool initialized;

public:
    static void init(const WindowConfig& config,
                     const std::map<std::string, TileMap>& newScenarios);

    static const TileMap& getTileMap(const std::string& name);

    static std::vector<std::string> listNames();

    static bool existsScenario(const std::string& name);

    static const WindowConfig& getWindowConfig();
};

#endif  // SCENARIO_REGISTRY_H_
