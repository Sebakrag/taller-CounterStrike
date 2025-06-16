#include "../include/scenario_registry.h"

#include <stdexcept>

std::map<std::string, TileMap> ScenarioRegistry::scenarios;
WindowConfig ScenarioRegistry::windowConfig;
bool ScenarioRegistry::initialized = false;

void ScenarioRegistry::init(const WindowConfig& config,
                            const std::map<std::string, TileMap>& newScenarios) {
    if (!initialized) {
        initialized = true;

        windowConfig = config;
        scenarios = newScenarios;
    }
}

const TileMap& ScenarioRegistry::getTileMap(const std::string& name) {
    auto it = scenarios.find(name);
    if (it == scenarios.end()) {
        throw std::runtime_error("Scenario '" + name + "' not found in ScenarioRegistry.");
    }
    return it->second;
}

std::vector<std::string> ScenarioRegistry::listNames() {
    std::vector<std::string> names;
    for (const auto& pair: scenarios) {
        names.push_back(pair.first);
    }
    return names;
}
bool ScenarioRegistry::existsScenario(const std::string& name) {
    auto it = scenarios.find(name);
    if (it == scenarios.end()) {  // no existe el escenario
        return false;
    }
    return true;
}
const WindowConfig& ScenarioRegistry::getWindowConfig() { return windowConfig; }
