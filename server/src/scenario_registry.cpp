#include "../include/scenario_registry.h"

#include <stdexcept>
#include <filesystem>
#include <yaml-cpp/yaml.h>

// inicializo constantes static para compilar
std::map<std::string, TileMap> ScenarioRegistry::scenarios;
WindowConfig ScenarioRegistry::windowConfig;
FOVConfig ScenarioRegistry::fovConfig;
bool ScenarioRegistry::initialized = false;

void ScenarioRegistry::init(const WindowConfig& win_config, const FOVConfig& fov_config,
                            const std::map<std::string, TileMap>& newScenarios) {
    if (!initialized) {
        initialized = true;

        windowConfig = win_config;
        fovConfig = fov_config;
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
const FOVConfig& ScenarioRegistry::getFovConfig() { return fovConfig; }

std::vector<std::string> ScenarioRegistry::listAvailableMaps(const std::string& mapsDirectory) {
    std::vector<std::string> availableMaps;
    
    try {
        std::filesystem::path dirPath(mapsDirectory);
        
        if (!std::filesystem::exists(dirPath)) {
            return availableMaps;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
                std::string mapName = entry.path().stem().string();
                availableMaps.push_back(mapName);
            }
        }
    } catch (const std::exception& e) {
    }
    
    return availableMaps;
}

bool ScenarioRegistry::loadMapFromYaml(const std::string& mapName, const std::string& mapsDirectory) {
    try {
        std::string filePath = mapsDirectory + "/" + mapName + ".yaml";
        YAML::Node mapNode = YAML::LoadFile(filePath);
        
        // Verificar que el nodo raíz existe
        if (!mapNode.IsMap()) {
            return false;
        }
        
        std::string yamlMapName = mapName;
        if (mapNode["name"] && mapNode["name"].IsScalar()) {
            yamlMapName = mapNode["name"].as<std::string>();
        }
        
        TileMap tileMap;
        
        if (mapNode["matrix"] && mapNode["matrix"].IsSequence()) {
            std::vector<std::vector<int>> matrix;
            
            for (const auto& row : mapNode["matrix"]) {
                if (row.IsSequence()) {
                    std::vector<int> rowData;
                    for (const auto& cell : row) {
                        rowData.push_back(cell.as<int>());
                    }
                    matrix.push_back(rowData);
                }
            }
            
            if (!matrix.empty() && !matrix[0].empty()) {
                tileMap = TileMap(matrix);
            } else {
                return false;
            }
        } else {
            return false;
        }
        
        scenarios[mapName] = tileMap;
        
        return true;
    } catch (const YAML::Exception& e) {
        return false;
    } catch (const std::exception& e) {
        return false;
    }
}
