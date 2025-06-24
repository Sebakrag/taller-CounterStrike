#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <cstdint>
#include <map>
#include <utility>
#include <vector>

#include "utils/Vec2D.h"

#include "tile.h"
#include "types.h"


#define TILE_SIZE 32

class TileMap {
private:
    TypeTileMap type;
    std::vector<std::vector<Tile>> matriz;  // matriz de tiles
    int row_count;
    int col_count;

    // zonas
    Vec2D pos_bomb_zone;
    Vec2D pos_t_zone;
    Vec2D pos_ct_zone;

    std::map<Vec2D, Weapon> weaponPositions;
    // inicialmente no puede haber 2 armas en una posicion

public:
    TileMap() {}

    // Constructor a partir de una matriz de id's de tiles
    explicit TileMap(const TypeTileMap& type, const std::vector<std::vector<int>>& matrizIdsTiles);

    explicit TileMap(const TypeTileMap& type, const std::vector<std::vector<int>>& matrizIdsTiles,
                     const std::map<int, TypeTile>& typesTiles,
                     const std::map<Vec2D, Weapon>& weaponPositions = {});

    // Constructor por copia
    TileMap(const TileMap& other);
    TileMap& operator=(const TileMap& other);


    int getIdTile(unsigned int row, unsigned int col) const;

    Tile getTile(unsigned int row, unsigned int col) const;

    int getRowCount() const;
    int getColCount() const;
    TypeTileMap getType() const;

    Vec2D getPosBombZone() const;
    Vec2D getPosTerroristZone() const;
    Vec2D getPosCounterTerroistZone() const;

    void print() const;  // para testear

    static TileMap getLevelDemo();
    // static TileMap getCurrentLevel();

    /**
     * Constructor a partir de una tira de bytes, donde
     * los primeros 4 bytes indican la cantidad de filas
     * y los siguientes 4 bytes la cantidad de columnas
     *
     */
    explicit TileMap(const std::vector<uint8_t>& buffer);

    /**
     * Serializa el tilemap en una tira de bytes,
     * donde los primeros 4 bytes indican la cantidad de filas,
     * y los siguientes 4 bytes la cantidad de columnas.
     *
     * (nota. este método solo guarda los ids de cada tile, pero no su type.
     * Sin enmbargo, este metodo solo se utiliza para enviar el buffer al
     * cliente a traves del protocolo, y ya que al cliente solo le interesan los ids,
     * no hay problema en perder esa información).
     */
    std::vector<uint8_t> toBytes() const;

    const std::map<Vec2D, Weapon>& getWeaponPositions() const { return weaponPositions; }
};


#endif  // TILE_MAP_H
