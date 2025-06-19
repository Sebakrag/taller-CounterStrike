#include "bomb_info.h"
#include "common/protocol.h"
#include <iostream>

BombInfo::BombInfo():
    serverId(0),
    state(BombState::Dropped),
    position(0,0) {}

BombInfo::BombInfo(uint32_t serverId, BombState state, const Vec2D& position):
    serverId(serverId),
    state(state),
    position(position) {}

std::vector<uint8_t> BombInfo::toBytes() const {
    std::vector<uint8_t> buffer;
    Protocol_::insertBigEndian32(serverId, buffer);
    buffer.push_back(Protocol_::encodeBombState(state));
    Protocol_::insertFloat4Bytes(position.getX(), buffer);
    Protocol_::insertFloat4Bytes(position.getY(), buffer);
    return buffer;
}

BombInfo::BombInfo(const std::vector<uint8_t> &bytes) {
    int index = 0;
    serverId = Protocol_::getBigEndian32(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    state = Protocol_::decodeBombState(bytes[index++]);

    float posX = Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    float posY = Protocol_::getFloat(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    position.set(posX, posY);
}

void BombInfo::print() const {
    std::cout << "===== Bomb Info =====" << std::endl;
    std::cout << "Server Entity ID: " << serverId << std::endl;
    std::cout << "State: " << static_cast<int>(state) << std::endl;
    std::cout << "Position: (" << position.getX() << ", " << position.getY() << ")" << std::endl;
    std::cout << "=====================" << std::endl;
}


