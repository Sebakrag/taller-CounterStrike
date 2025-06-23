#include "bomb_info.h"

#include <stdexcept>

BombInfo::BombInfo(const ServerEntityID server_entt_id, const BombState state, const float pos_x,
                   const float pos_y):
        server_entt_id(server_entt_id), state(state), pos_x(pos_x), pos_y(pos_y) {}

BombInfo::BombInfo(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != SIZE_BOMB_INFO) {
        throw std::runtime_error("Error en constructor BombInfo(const std::vector<uint8_t>& "
                                 "bytes). El buffer no tiene los bytes esperados.");
    }
    // EntityID (4bytes)
    server_entt_id = Protocol_::getBigEndian32(bytes[0], bytes[1], bytes[2], bytes[3]);

    // State
    state = Protocol_::decodeBombState(bytes[4]);

    // pos (4 bytes cada uno)
    pos_x = Protocol_::getFloat(bytes[5], bytes[6], bytes[7], bytes[8]);
    pos_y = Protocol_::getFloat(bytes[9], bytes[10], bytes[11], bytes[12]);
}

SpriteType BombInfo::getSpriteType() const { return SpriteType::BOMB; }

std::vector<uint8_t> BombInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    // EntityID (4 bytes)
    Protocol_::insertBigEndian32(server_entt_id, buffer);
    // State (1 byte)
    buffer.push_back(Protocol_::encodeBombState(state));
    // pos (4 bytes cada uno)
    Protocol_::insertFloat4Bytes(pos_x, buffer);
    Protocol_::insertFloat4Bytes(pos_y, buffer);

    return buffer;
}
