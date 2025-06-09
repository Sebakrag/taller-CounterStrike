#include "../../../client/include/model/Map.h"

#include <cmath>
#include <iostream>

#include "../../../client/client_constants.h"
#include "../../../client/include/model/Graphics.h"
#include "../../../client/include/model/utils/TextureManager.h"

using SDL2pp::Optional;

Map::Map(const TileMap& tileMap):
        srcTileMap((tileMap.getRowCount()), std::vector<Rect>((tileMap.getColCount()))),
        tileSetTexture(TextureManager::getTextureMap(tileMap.getType())),
        widthInTiles(tileMap.getColCount()),
        heightInTiles(tileMap.getRowCount()) {
    if (!tileSetTexture) {
        throw std::runtime_error("TextureManager::getTexture returned nullptr!");
    }

    const int TILES_PER_ROW = (tileSetTexture->GetWidth() / TILE_SIZE);
    std::cout << "heigth: " << heightInTiles << std::endl;
    for (int y = 0; y < heightInTiles; ++y) {
        for (int x = 0; x < widthInTiles; ++x) {
            const int tileID = tileMap.getIdTile(y, x);
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

void Map::render(Graphics& graphics, const Camera& camera) {
    const Rect view = camera.getViewport();

    // Determinar tiles visibles (en coordenadas de tiles, no píxeles)
    const int startTileX = view.GetX() / TILE_SIZE;
    const int startTileY = view.GetY() / TILE_SIZE;

    // Usamos ceil para asegurarnos de cubrir fracciones visibles
    const int endTileX = static_cast<int>(
            std::ceil((view.GetX() + view.GetW()) / static_cast<float>(TILE_SIZE)));
    const int endTileY = static_cast<int>(
            std::ceil((view.GetY() + view.GetH()) / static_cast<float>(TILE_SIZE)));

    const Vec2D cameraOffset = camera.getOffset();

    for (int tileY = startTileY; (tileY < endTileY) && (tileY < heightInTiles); ++tileY) {
        if (tileY < 0 || tileY >= heightInTiles)
            continue;

        for (int tileX = startTileX; (tileX < endTileX) && (tileX < widthInTiles); ++tileX) {
            if (tileX < 0 || tileX >= widthInTiles)
                continue;

            const Rect& srcRect = srcTileMap[tileY][tileX];

            // Posición en pantalla en píxeles (ajustada por la cámara)
            int screenX = tileX * TILE_SIZE - static_cast<int>(cameraOffset.getX());
            int screenY = tileY * TILE_SIZE - static_cast<int>(cameraOffset.getY());

            Rect destRect(screenX, screenY, TILE_SIZE, TILE_SIZE);
            graphics.draw(*tileSetTexture, Optional<Rect>(srcRect), Optional<Rect>(destRect));
        }
    }
}
