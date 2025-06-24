#pragma once

#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QString>

// Tipos de elementos del mapa
enum ElementType {
    TEAM_SPAWN_CT = 0,
    TEAM_SPAWN_T = 1,
    BOMB_ZONE = 2,
    SOLID_STRUCTURE = 3,
    WEAPON = 4,
    TILE = 5,       // Nuevo tipo para los tiles del mapa
    EXTRA_TILE = 6  // Tipo para los tiles extra
};

// Tipos de terreno
enum TerrainType { DESERT = 0, AZTEC_VILLAGE = 1, TRAINING_ZONE = 2 };

// Tipos de armas
enum WeaponType { PISTOL = 0, RIFLE = 1, SNIPER = 2, SHOTGUN = 3 };

class MapElement {
public:
    MapElement(const QPointF& pos, int type);
    virtual ~MapElement() = default;

    virtual int getType() const;
    virtual QPointF getPosition() const;
    virtual void setPosition(const QPointF& pos);

protected:
    QPointF position;
    int elementType;
};

class TeamSpawn: public MapElement {
public:
    TeamSpawn(const QPointF& pos, int teamId);

    int getTeamId() const;
    void setTeamId(int id);

private:
    int teamId;  // 0 = CT, 1 = T
};

class BombZone: public MapElement {
public:
    explicit BombZone(const QPointF& pos, const QSizeF& size = QSizeF(50, 50));

    QSizeF getSize() const;
    void setSize(const QSizeF& size);

private:
    QSizeF size;
};

class SolidStructure: public MapElement {
public:
    SolidStructure(const QPointF& pos, int structureType);

    int getStructureType() const;
    void setStructureType(int type);

private:
    int structureType;  // Tipo de estructura sólida
};

class Weapon: public MapElement {
public:
    Weapon(const QPointF& pos, int weaponType);

    int getWeaponType() const;
    void setWeaponType(int type);

private:
    int weaponType;  // Tipo de arma
};

class Tile: public MapElement {
public:
    Tile(const QPointF& pos, int tileId);

    int getTileId() const;
    void setTileId(int id);

private:
    int tileId;  // ID del tile (corresponde al índice en tilePixmaps)
};
