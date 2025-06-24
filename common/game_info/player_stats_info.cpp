#include "player_stats_info.h"

#include "server/include/player.h"

PlayerStatsInfo::PlayerStatsInfo(const std::string& username, int kills, int deaths, int moneyEarned)
    : username(username), kills(kills), deaths(deaths), moneyEarned(moneyEarned) {}

PlayerStatsInfo::PlayerStatsInfo(const std::vector<uint8_t> &bytes) {
    size_t index = 0;

    //leo la longitud del username y luego el username
    int lengthName = Protocol_::getValueBigEndian16(bytes[index], bytes[index + 1]);
    index += 2;

    username = std::string(bytes.begin() + index, bytes.begin() + index + lengthName);
    index += lengthName;

    kills = Protocol_::getBigEndian32(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    deaths = Protocol_::getBigEndian32(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
    moneyEarned = Protocol_::getBigEndian32(bytes[index], bytes[index + 1], bytes[index + 2], bytes[index + 3]);
    index += 4;
}

std::vector<uint8_t> PlayerStatsInfo::toBytes() const {
    std::vector<uint8_t> buffer;

    const int lengthName = static_cast<int>(username.length());
    Protocol_::insertBigEndian16(lengthName, buffer);
    Protocol_::insertStringBytes(username, buffer);

    Protocol_::insertBigEndian32(kills, buffer);
    Protocol_::insertBigEndian32(deaths, buffer);
    Protocol_::insertBigEndian32(moneyEarned, buffer);

    return buffer;
}
