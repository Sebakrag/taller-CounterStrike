#include "../include/game_info.h"
#include <sstream>

std::string GameInfo::toDebugString() const {
    std::stringstream ss;
    ss << "Jugador en (" << posX << "," << posY << "), HP: " << health << "\n";
    ss << "Arma equipada: " << static_cast<int>(equippedWeapon)
       << ", balas: " << bullets << "\n";
    ss << "Bomba: " << (bombPlanted ? "PLANTADA" : "no plantada") << "\n";
    ss << "Tiempo restante: " << timeLeft << "\n";
    ss << "Otros jugadores visibles:\n";
    for (const auto& p : otherPlayers) {
        ss << " - ID " << p.id << " (" << p.x << "," << p.y << ") "
           << (p.isAlive ? "vivo" : "muerto") << "\n";
    }
    return ss.str();
}