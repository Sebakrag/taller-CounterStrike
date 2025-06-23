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
            std::cerr << "ERROR - El directorio de mapas no existe: '" << dirPath.string() << "'" << std::endl;
            return availableMaps;
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".yaml") {
                std::string mapName = entry.path().stem().string();
                availableMaps.push_back(mapName);
            }
        }

        std::cerr << "DEBUG - Mapas disponibles en '" << mapsDirectory << "':\n";

    } catch (const std::exception& e) {
    }
    
    return availableMaps;
}

// Definir un comparador personalizado para Vec2D
struct Vec2DCompare {
    bool operator()(const Vec2D& a, const Vec2D& b) const {
        if (a.getX() != b.getX()) {
            return a.getX() < b.getX();
        }
        return a.getY() < b.getY();
    }
};

bool ScenarioRegistry::loadMapFromYaml(const std::string& mapName, const std::string& mapsDirectory) {
    try {
        std::string filePath = mapsDirectory + "/" + mapName + ".yaml";
        YAML::Node mapNode = YAML::LoadFile(filePath);
        
        // Verificar que el nodo raíz existe
        if (!mapNode.IsMap()) {
            return false;
        }
        
        // Obtener el tipo de mapa
        TypeTileMap mapType = TypeTileMap::Desert; // Por defecto
        if (mapNode["map_type"] && mapNode["map_type"].IsScalar()) {
            std::string mapTypeStr = mapNode["map_type"].as<std::string>();
            // Convertir string a TypeTileMap (puedes expandir según los tipos disponibles)
            if (mapTypeStr == "Desert") {
                mapType = TypeTileMap::Desert;
            } else if (mapTypeStr == "Aztec") {
                mapType = TypeTileMap::Aztec;
            } else if (mapTypeStr == "Training") {
                mapType = TypeTileMap::Training;
            }
        }
        
        // Obtener la matriz
        std::vector<std::vector<int>> matrix;
        if (mapNode["matrix"] && mapNode["matrix"].IsSequence()) {
            for (const auto& row : mapNode["matrix"]) {
                if (row.IsSequence()) {
                    std::vector<int> rowData;
                    for (const auto& cell : row) {
                        rowData.push_back(cell.as<int>());
                    }
                    matrix.push_back(rowData);
                }
            }
        } else {
            return false;
        }
        
        if (matrix.empty() || matrix[0].empty()) {
            return false;
        }
        
        // Construir el mapa de tipos de tiles
        std::map<int, TypeTile> tileTypes;
        if (mapNode["tiles"] && mapNode["tiles"].IsSequence()) {
            for (const auto& tileNode : mapNode["tiles"]) {
                if (tileNode["id"] && tileNode.IsMap()) {
                    int tileId = tileNode["id"].as<int>();
                    if (tileNode["type"] && tileNode["type"].IsScalar()) {
                        std::string typeStr = tileNode["type"].as<std::string>();
                        TypeTile type = TypeTile::None; // Por defecto
                        
                        // Convertir string a TypeTile
                        if (typeStr == "solid") {
                            type = TypeTile::Solid;
                        } else if (typeStr == "") {
                            type = TypeTile::None;
                        } else if (typeStr == "ct_zone") {
                            type = TypeTile::CT_Zone;
                        } else if (typeStr == "t_zone") {
                            type = TypeTile::T_Zone;
                        } else if (typeStr == "bomb_zone") {
                            type = TypeTile::BombZone;
                        }
                        
                        tileTypes[tileId] = type;
                    } else {
                        // Si no se especifica el tipo, asumimos solid para barreras y empty para el resto
                        tileTypes[tileId] = (tileId == 4) ? TypeTile::Solid : TypeTile::None;
                    }
                }
            }
        }
        
        // Construir el mapa de armas (opcional)
        std::map<Vec2D, Weapon, Vec2DCompare> weaponsMap;
        if (mapNode["weapons"] && mapNode["weapons"].IsSequence()) {
            for (const auto& weaponNode : mapNode["weapons"]) {
                if (weaponNode.IsMap() && weaponNode["pos"] && weaponNode["type"]) {
                    int x = weaponNode["pos"][0].as<int>();
                    int y = weaponNode["pos"][1].as<int>();
                    Vec2D pos(x, y);
                    
                    std::string weaponType = weaponNode["type"].as<std::string>();
                    Weapon weapon = Weapon::Knife; // Por defecto
                    
                    // Convertir string a Weapon
                    if (weaponType == "ak47") {
                        weapon = Weapon::Ak47;
                    } else if (weaponType == "awp") {
                        weapon = Weapon::Awp;
                    } else if (weaponType == "m3") {
                        weapon = Weapon::M3;
                    } else if (weaponType == "glock") {
                        weapon = Weapon::Glock;
                    }
                    
                    weaponsMap[pos] = weapon;
                }
            }
        }
        
        // Sin embargo, necesitamos un std::map<Vec2D, Weapon> sin comparador personalizado
        // para pasarlo al constructor de TileMap. Convertimos nuestro mapa:
        std::map<Vec2D, Weapon> standardWeaponsMap;
        for (const auto& pair : weaponsMap) {
            standardWeaponsMap.insert(std::make_pair(pair.first, pair.second));
        }
        
        // Crear el TileMap con todos los datos obtenidos
        TileMap tileMap(mapType, matrix, tileTypes, standardWeaponsMap);
        
        // Guardar en el registro de escenarios
        scenarios[mapName] = tileMap;
        
        return true;
    } catch (const YAML::Exception& e) {
        return false;
    } catch (const std::exception& e) {
        return false;
    }
}
