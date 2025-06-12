#include "mapElements.h"

// MapElement implementation
MapElement::MapElement(const QPointF &pos, int type) : position(pos), elementType(type) {}

int MapElement::getType() const {
    return elementType;
}

QPointF MapElement::getPosition() const {
    return position;
}

void MapElement::setPosition(const QPointF &pos) {
    position = pos;
}

// TeamSpawn implementation
TeamSpawn::TeamSpawn(const QPointF &pos, int team) 
    : MapElement(pos, team == 0 ? TEAM_SPAWN_CT : TEAM_SPAWN_T), teamId(team) {}

int TeamSpawn::getTeamId() const {
    return teamId;
}

void TeamSpawn::setTeamId(int id) {
    teamId = id;
    elementType = id == 0 ? TEAM_SPAWN_CT : TEAM_SPAWN_T;
}

// BombZone implementation
BombZone::BombZone(const QPointF &pos, const QSizeF &zoneSize) 
    : MapElement(pos, BOMB_ZONE), size(zoneSize) {}

QSizeF BombZone::getSize() const {
    return size;
}

void BombZone::setSize(const QSizeF &zoneSize) {
    size = zoneSize;
}

// SolidStructure implementation
SolidStructure::SolidStructure(const QPointF &pos, int type) 
    : MapElement(pos, SOLID_STRUCTURE), structureType(type) {}

int SolidStructure::getStructureType() const {
    return structureType;
}

void SolidStructure::setStructureType(int type) {
    structureType = type;
}

// Weapon implementation
Weapon::Weapon(const QPointF &pos, int type) 
    : MapElement(pos, WEAPON), weaponType(type) {}

int Weapon::getWeaponType() const {
    return weaponType;
}

void Weapon::setWeaponType(int type) {
    weaponType = type;
}

// Tile implementation
Tile::Tile(const QPointF &pos, int id) 
    : MapElement(pos, TILE), tileId(id) {}

int Tile::getTileId() const {
    return tileId;
}

void Tile::setTileId(int id) {
    tileId = id;
}


