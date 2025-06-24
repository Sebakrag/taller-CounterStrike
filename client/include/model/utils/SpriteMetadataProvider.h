#ifndef SPRITEMETADATAPROVIDER_H
#define SPRITEMETADATAPROVIDER_H

#include "SpriteType.h"

struct SpriteSheetLayout {
    const int numSpritesAlongWidth;   // Cant de sprites que hay por fila
    const int numSpritesAlongHeight;  // Cant de sprites que hay por columna

    SpriteSheetLayout(const int cols, const int rows):
            numSpritesAlongWidth(cols), numSpritesAlongHeight(rows) {}
};

class SpriteMetadataProvider {
private:
    SpriteMetadataProvider() = delete;

public:
    static SpriteSheetLayout getLayout(SpriteType type);
};

#endif  // SPRITEMETADATAPROVIDER_H
