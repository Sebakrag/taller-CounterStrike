#include "client/include/model/Map.h"

#include "client/client_constants.h"
#include "client/include/model/utils/TextureManager.h"

using SDL2pp::Optional;

Map::Map(const MapInfo& mapInfo):
        srcTileMap(mapInfo.height, std::vector<Rect>(mapInfo.width)),
        tileSetTexture(TextureManager::getTexture(mapInfo.tileSetType)),
        width(mapInfo.width),
        height(mapInfo.height),
        onMapRect(0, 0, TILE_SIZE, TILE_SIZE) {
    const int TILES_PER_ROW = (tileSetTexture->GetWidth() / TILE_SIZE);

    const auto& tileMap = mapInfo.tileMap;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileID = tileMap[y][x];
            Rect srcRect(0, 0, 0, 0);
            if (tileID > 0) {
                // Le resto 1 pq enumeramos los tiles de 1 en adelante.
                srcRect.SetX(((tileID - 1) % TILES_PER_ROW) * TILE_SIZE);
                srcRect.SetY(((tileID - 1) / TILES_PER_ROW) * TILE_SIZE);
                srcRect.SetW(TILE_SIZE);
                srcRect.SetH(TILE_SIZE);
            }

            srcTileMap[y][x] = srcRect;
        }
    }
}

void Map::render(Renderer& ren) {
    for (int y = 0; y < height; ++y) {
        onMapRect.SetY(TILE_SIZE * y);
        for (int x = 0; x < width; ++x) {
            onMapRect.SetX(TILE_SIZE * x);
            ren.Copy(*tileSetTexture, Optional{srcTileMap[y][x]}, Optional{onMapRect});
        }
    }
}
