#include "../../../server/include/weapon/bomb.h"


//-------------
// Inicializo las variables estáticas (para poder compilar)
bool Bomb::initialized = false;
double Bomb::TIME_TO_EXPLODE = 0;


void Bomb::init(double time_to_explode) {
    if (initialized == false) {
        TIME_TO_EXPLODE = time_to_explode;
        initialized = true;
    }
}
//---------

Bomb::Bomb():
        state(BombState::Carried),
        posX(0.0f),
        posY(0.0f),
        plantedPosition(0.0f, 0.0f),
        carrierId(""),
        timer(0.0),
        timeToExplode(TIME_TO_EXPLODE),
        serverId(IdGenerator::getNextId()) {}

void Bomb::assignTo(const std::string& playerId) {
    state = BombState::Carried;
    carrierId = playerId;
}

void Bomb::reset() {
    state = BombState::Carried;
    carrierId.clear();
    posX = posY = 0.0f;
    plantedPosition = Vec2D(0.0f, 0.0f);
    timer = 0.0;
}

bool Bomb::isCarriedBy(const std::string& playerId) const {
    return state == BombState::Carried && carrierId == playerId;
}

bool Bomb::hasExploded() const { return state == BombState::Exploded; }

bool Bomb::isDefused() const { return state == BombState::Defused; }

bool Bomb::isPlanted() const { return state == BombState::Planted; }

bool Bomb::isDropped() const { return state == BombState::Dropped; }

bool Bomb::isCarried() const { return state == BombState::Carried; }

void Bomb::drop(float x, float y) {
    if (state != BombState::Carried)
        return;
    posX = x;
    posY = y;
    carrierId.clear();
    state = BombState::Dropped;
}

void Bomb::pickUp(const std::string& playerId) {
    if (state != BombState::Dropped)
        return;
    carrierId = playerId;
    state = BombState::Carried;
}

bool Bomb::plant(float x, float y, Map& map) {
    if (state != BombState::Carried)
        return false;

    if (!map.isBombZone(static_cast<int>(x), static_cast<int>(y)))
        return false;

    plantedPosition = Vec2D(x, y);
    posX = x;
    posY = y;
    carrierId.clear();
    state = BombState::Planted;
    timer = timeToExplode;
    return true;
}

bool Bomb::defuse() {
    if (state != BombState::Planted)
        return false;

    state = BombState::Defused;
    timer = 0.0;
    return true;
}

void Bomb::update(double elapsedTime) {
    if (state == BombState::Planted) {
        timer -= elapsedTime;
        if (timer <= 0.0) {
            timer = 0.0;
            state = BombState::Exploded;
        }
    }
}

float Bomb::getX() const {
    return posX;
}

float Bomb::getY() const {
    return posY;
}

Vec2D Bomb::getPlantedPosition() const {
    return plantedPosition;
}

std::string Bomb::getCarrierId() const {
    return carrierId;
}

BombState Bomb::getState() const {
    return state;
}

uint32_t Bomb::getServerId() const {
    return serverId;
}

double Bomb::getTimer() const {
    return timer;
}

