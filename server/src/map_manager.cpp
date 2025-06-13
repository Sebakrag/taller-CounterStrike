#include "../include/map_manager.h"
#include <filesystem>
#include <fstream>
#include <iostream>

MapManager::MapManager(const std::string& mapsFolderPath)
    : mapsFolderPath(mapsFolderPath) {
    updateAvailableMaps();
}

void MapManager::updateAvailableMaps() {
    availableMaps.clear();
    
    // Verificar si existe la carpeta
    if (!std::filesystem::exists(mapsFolderPath)) {
        std::filesystem::create_directories(mapsFolderPath);
        return;
    }
    
    // Recorrer la carpeta y procesar cada archivo YAML
    for (const auto& entry : std::filesystem::directory_iterator(mapsFolderPath)) {
        if (entry.path().extension() == ".yaml" || entry.path().extension() == ".yml") {
            try {
                MapInfo mapInfo = getMapInfo(entry.path().string());
                availableMaps.push_back(mapInfo);
            } catch (const std::exception& e) {
                std::cerr << "Error al procesar el archivo " 
                          << entry.path().string() << ": " << e.what() << std::endl;
            }
        }
    }
}

std::vector<MapInfo> MapManager::getAvailableMaps() const {
    return availableMaps;
}

MapInfo MapManager::getMapInfo(const std::string& filePath) const {
    YAML::Node mapNode = YAML::LoadFile(filePath);
    
    MapInfo info;
    info.fileName = std::filesystem::path(filePath).filename().string();
    
    if (mapNode["map"]) {
        YAML::Node map = mapNode["map"];
        
        if (map["name"]) {
            info.name = map["name"].as<std::string>();
        } else {
            info.name = info.fileName;
        }
        
        if (map["application_info"]) {
            YAML::Node appInfo = map["application_info"];
            
            if (appInfo["map_type"]) {
                std::string typeStr = appInfo["map_type"].as<std::string>();
                if (typeStr == "Desert") {
                    info.type = TypeTileMap::Desert;
                } else if (typeStr == "Aztec") {
                    info.type = TypeTileMap::Aztec;
                } else if (typeStr == "Training") {
                    info.type = TypeTileMap::Training;
                } else {
                    info.type = TypeTileMap::Desert;  // Default
                }
            }
            
            if (appInfo["width"] && appInfo["height"]) {
                info.width = appInfo["width"].as<int>();
                info.height = appInfo["height"].as<int>();
            }
        }
    }
    
    return info;
}

TileMap MapManager::loadMap(const std::string& fileName) const {
    std::string filePath = mapsFolderPath + "/" + fileName;
    YAML::Node mapNode = YAML::LoadFile(filePath);
    
    if (mapNode["map"]) {
        return convertYamlToTileMap(mapNode["map"]);
    }
    
    throw std::runtime_error("Formato de mapa inválido");
}

TileMap MapManager::convertYamlToTileMap(const YAML::Node& mapNode) const {
    // Obtener el tipo de mapa
    TypeTileMap type = TypeTileMap::Desert;  // Default
    if (mapNode["application_info"] && mapNode["application_info"]["map_type"]) {
        std::string typeStr = mapNode["application_info"]["map_type"].as<std::string>();
        if (typeStr == "Desert") {
            type = TypeTileMap::Desert;
        } else if (typeStr == "Aztec") {
            type = TypeTileMap::Aztec;
        } else if (typeStr == "Training") {
            type = TypeTileMap::Training;
        }
    }
    
    // Obtener la matriz de tiles
    std::vector<std::vector<int>> tileMatrix;
    if (mapNode["tile_matrix"]) {
        YAML::Node matrixNode = mapNode["tile_matrix"];
        for (const auto& row : matrixNode) {
            std::vector<int> tileRow;
            for (const auto& tile : row) {
                tileRow.push_back(tile.as<int>());
            }
            tileMatrix.push_back(tileRow);
        }
    } else {
        // Si no hay matriz, crear una predeterminada
        tileMatrix = {{1, 1}, {1, 1}};
    }
    
    return TileMap(type, tileMatrix);
}
