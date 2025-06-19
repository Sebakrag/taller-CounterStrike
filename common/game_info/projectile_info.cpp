#include "projectile_info.h"

ProjectileInfo::ProjectileInfo():
    x(0), y(0),
    dirX(0), dirY(0),
    shooterUsername(""),
    weapon(Weapon::None),
    active(false) {}

ProjectileInfo::ProjectileInfo(float x, float y, float dirX, float dirY, const std::string &shooterUsername, Weapon weapon, bool active):
    x(x), y(y),
    dirX(dirX), dirY(dirY),
    shooterUsername(shooterUsername),
    weapon(weapon),
    active(active) {}

std::vector<uint8_t> ProjectileInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    Protocol_::insertFloat4Bytes(x, buffer);
    Protocol_::insertFloat4Bytes(y, buffer);
    Protocol_::insertFloat4Bytes(dirX, buffer);
    Protocol_::insertFloat4Bytes(dirY, buffer);

    Protocol_::insertBigEndian16(shooterUsername.size(), buffer);
    Protocol_::insertStringBytes(shooterUsername, buffer);

    buffer.push_back(Protocol_::encodeWeapon(weapon));
    buffer.push_back(Protocol_::encodeBool(active));

    return buffer;
}

ProjectileInfo::ProjectileInfo(const std::vector<uint8_t> &bytes) {
    int index = 0;
    x = Protocol_::getFloat(bytes[index], bytes[index+1], bytes[index+2], bytes[index+3]);
    index += 4;
    y = Protocol_::getFloat(bytes[index], bytes[index+1], bytes[index+2], bytes[index+3]);
    index += 4;
    dirX = Protocol_::getFloat(bytes[index], bytes[index+1], bytes[index+2], bytes[index+3]);
    index += 4;
    dirY = Protocol_::getFloat(bytes[index], bytes[index+1], bytes[index+2], bytes[index+3]);
    index += 4;

    uint16_t len = Protocol_::getValueBigEndian16(bytes[index], bytes[index+1]);
    index += 2;
    shooterUsername = std::string(bytes.begin() + index, bytes.begin() + index + len);
    index += len;

    weapon = Protocol_::decodeWeapon(bytes[index++]);
    active = Protocol_::decodeBool(bytes[index++]);
}

void ProjectileInfo::print() const {
    std::cout << "Projectile | Pos: (" << x << "," << y << "), Dir: (" << dirX << "," << dirY
              << "), Shooter: " << shooterUsername
              << ", Weapon: " << static_cast<int>(weapon)
              << ", Active: " << active << std::endl;
}