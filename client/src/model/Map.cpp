#include "../../../client/include/model/Map.h"

#include <cmath>

#include "../../../client/client_constants.h"
#include "../../../client/include/model/Graphics.h"
#include "../../../client/include/model/utils/TextureManager.h"

using SDL2pp::Optional;

Map::Map(const TileMap& tileMap, Graphics& graphics):
        srcTileMap(tileMap.getRowCount(), std::vector<Rect>(tileMap.getColCount())),
        tileSetTexture(TextureManager::getTextureMap(tileMap.getType())),
        widthInTiles(tileMap.getColCount()),
        heightInTiles(tileMap.getRowCount()) {

    if (!tileSetTexture) {
        throw std::runtime_error("TextureManager::getTexture returned nullptr!");
    }

    const int mapWidthPx = widthInTiles * TILE_SIZE;
    const int mapHeightPx = heightInTiles * TILE_SIZE;

    // Crear la textura del mapa pre-renderizado
    mapTexture = graphics.createTargetTexture(mapWidthPx, mapHeightPx);


    // Crear el srcTileMap y renderizarlo a mapTexture
    const int TILES_PER_ROW = tileSetTexture->GetWidth() / TILE_SIZE;

    graphics.setRenderTarget(*mapTexture);
    graphics.clearWithTransparentBlack();

    for (int y = 0; y < heightInTiles; ++y) {
        for (int x = 0; x < widthInTiles; ++x) {
            const int tileID = tileMap.getIdTile(y, x);
            Rect srcRect(0, 0, 0, 0);

            if (tileID > 0) {
                srcRect.SetX(((tileID - 1) % TILES_PER_ROW) * TILE_SIZE);
                srcRect.SetY(((tileID - 1) / TILES_PER_ROW) * TILE_SIZE);
                srcRect.SetW(TILE_SIZE);
                srcRect.SetH(TILE_SIZE);
            }

            srcTileMap[y][x] = srcRect;

            if (srcRect.GetW() > 0 && srcRect.GetH() > 0) {
                Rect dest(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
                graphics.draw(*tileSetTexture, Optional<Rect>(srcRect), Optional<Rect>(dest));
            }
        }
    }
    graphics.resetRenderTarget();
}

// Map::Map(const TileMap& tileMap):
//         srcTileMap((tileMap.getRowCount()), std::vector<Rect>((tileMap.getColCount()))),
//         tileSetTexture(TextureManager::getTextureMap(tileMap.getType())),
//         widthInTiles(tileMap.getColCount()),
//         heightInTiles(tileMap.getRowCount()) {
//     if (!tileSetTexture) {
//         throw std::runtime_error("TextureManager::getTexture returned nullptr!");
//     }
//
//     const int TILES_PER_ROW = (tileSetTexture->GetWidth() / TILE_SIZE);
//     for (int y = 0; y < heightInTiles; ++y) {
//         for (int x = 0; x < widthInTiles; ++x) {
//             const int tileID = tileMap.getIdTile(y, x);
//             Rect srcRect(0, 0, 0, 0);
//             if (tileID > 0) {
//                 // Le resto 1 pq enumeramos los tiles de 1 en adelante.
//                 srcRect.SetX(((tileID - 1) % TILES_PER_ROW) * TILE_SIZE);
//                 srcRect.SetY(((tileID - 1) / TILES_PER_ROW) * TILE_SIZE);
//                 srcRect.SetW(TILE_SIZE);
//                 srcRect.SetH(TILE_SIZE);
//             }
//             srcTileMap[y][x] = srcRect;
//         }
//     }
// }
void Map::render2(Graphics& graphics, const Camera& camera) const {
    const Rect view = camera.getViewport();
    const int mapW = widthInTiles * TILE_SIZE;
    const int mapH = heightInTiles * TILE_SIZE;

    // Coordenadas de src dentro de la textura
    int srcX = view.GetX();
    int srcY = view.GetY();
    int drawW = view.GetW();
    int drawH = view.GetH();

    // Coordenadas de dst en la ventana
    int dstX = 0;
    int dstY = 0;

    // Si la cámara se sale por la izquierda:
    if (srcX < 0) {
        dstX = -srcX;   // empezamos a dibujar más a la derecha
        drawW -= dstX;  // ancho efectivo
        srcX = 0;       // ya no pedimos < 0
    }
    // Si se sale por arriba:
    if (srcY < 0) {
        dstY = -srcY;
        drawH -= dstY;
        srcY = 0;
    }
    // Si se sale por la derecha:
    if (srcX + drawW > mapW) {
        drawW = mapW - srcX;
    }
    // Si se sale por abajo:
    if (srcY + drawH > mapH) {
        drawH = mapH - srcY;
    }

    // Si no queda nada que dibujar, salimos.
    if (drawW <= 0 || drawH <= 0)
        return;

    // Primero, la ventana ya está limpiada por Graphics::render (renderer.Clear()).
    // Sólo dibujamos la parte válida de la textura:
    Rect srcRect(srcX, srcY, drawW, drawH);
    Rect dstRect(dstX, dstY, drawW, drawH);
    graphics.draw(*mapTexture, Optional<Rect>(srcRect), Optional<Rect>(dstRect));
}

void Map::render(Graphics& graphics, const Camera& camera) const {
    const Rect view = camera.getViewport();
    // const Vec2D offset = camera.getOffset();

    Rect src(view.GetX(), view.GetY(), view.GetW(), view.GetH());
    Rect dst(view.GetX(), view.GetY(), widthInTiles * TILE_SIZE, heightInTiles * TILE_SIZE);

    graphics.draw(*mapTexture, src, dst);  // <-- esto llama a TU método
}


// void Map::render(Graphics& graphics, const Camera& camera) const {
//     const Rect view = camera.getViewport();
//
//     // Determinar tiles visibles (en coordenadas de tiles, no píxeles)
//     const int startTileX = view.GetX() / TILE_SIZE;
//     const int startTileY = view.GetY() / TILE_SIZE;
//
//     // Usamos ceil para asegurarnos de cubrir fracciones visibles
//     const int endTileX = static_cast<int>(
//             std::ceil((view.GetX() + view.GetW()) / static_cast<float>(TILE_SIZE)));
//     const int endTileY = static_cast<int>(
//             std::ceil((view.GetY() + view.GetH()) / static_cast<float>(TILE_SIZE)));
//
//     const Vec2D cameraOffset = camera.getOffset();
//
//     for (int tileY = startTileY; (tileY < endTileY) && (tileY < heightInTiles); ++tileY) {
//         if (tileY < 0 || tileY >= heightInTiles)
//             continue;
//
//         for (int tileX = startTileX; (tileX < endTileX) && (tileX < widthInTiles); ++tileX) {
//             if (tileX < 0 || tileX >= widthInTiles)
//                 continue;
//
//             const Rect& srcRect = srcTileMap[tileY][tileX];
//
//             // Posición en pantalla en píxeles (ajustada por la cámara)
//             int screenX = tileX * TILE_SIZE - static_cast<int>(cameraOffset.getX());
//             int screenY = tileY * TILE_SIZE - static_cast<int>(cameraOffset.getY());
//
//             Rect destRect(screenX, screenY, TILE_SIZE, TILE_SIZE);
//             graphics.draw(*tileSetTexture, Optional<Rect>(srcRect), Optional<Rect>(destRect));
//         }
//     }
// }
