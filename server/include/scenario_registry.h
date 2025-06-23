#ifndef SCENARIO_REGISTRY_H_
#define SCENARIO_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "../../common/dtos/FovConfig.h"
#include "../../common/dtos/WindowConfig.h"
#include "../../common/tile_map.h"

class ScenarioRegistry {
private:
    static std::map<std::string, TileMap> scenarios;
    static WindowConfig windowConfig;
    static FOVConfig
            fovConfig;  // esto podría estar cargado en otra clase pero no se me ocurrió otro lugar
    static bool initialized;

public:
    static void init(const WindowConfig& win_config, const FOVConfig& fov_config,
                     const std::map<std::string, TileMap>& newScenarios);

    static const TileMap& getTileMap(const std::string& name);

    static std::vector<std::string> listNames();

    static bool existsScenario(const std::string& name);

    static const WindowConfig& getWindowConfig();
    static const FOVConfig& getFovConfig();

    // Método para listar los archivos YAML de mapas disponibles
    static std::vector<std::string> listAvailableMaps(const std::string& mapsDirectory = "server/maps");

    // Método para cargar los mapas disponibles
    static void loadAvailableMaps(const std::string& mapsDirectory = "server/mapss");

    // Método para cargar un mapa específico desde YAML
    static bool loadMapFromYaml(const std::string& mapName, const std::string& mapsDirectory = "server/maps");
    
};

#endif  // SCENARIO_REGISTRY_H_
