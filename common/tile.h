#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <vector>

// se me ocurren 2 maneras para poder identificar si el Tile es solido a partir del ID.
// 1. Defino como constantes todos los ids de los tiles solidos.
//    Luego en el constructor, chequeo si es alguno de esos
// 2. Hacemos que los que tengan id impar sean sólidos y los que tengan id par no lo sean.
//    (habría que revisar los nombres de las imagenes con atención)
// [DISCUTIRLO]

// el id de cada tile corresponderá a un archivo png.
// por ej. id = 2 corresponde al archivo "tile_002.png"
//(no usar mas de 255 imagenes)

#define ID_WALL1 2  // pared

// 32x32 px
class Tile {
private:
    int id_tile;
    bool is_solid;  // dato que le sirve al server

public:
    explicit Tile(int id_tile);
    explicit Tile(int id_tile, bool is_solid);

    bool isSolid() const;

    int getIdTile() const;
};

#endif  // TILE_H
