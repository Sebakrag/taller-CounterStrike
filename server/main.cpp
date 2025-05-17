
#include <iostream>
#include "server.h"
#include "player.h"
#include "map.h"
#include "weapon/weapon_ak47.h"
#include "enums/player_type.h"
#include "enums/weapon_type.h"

int main(/*const int argc, const char* argv[]*/) {
    /*if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    const std::string port = argv[1];

    try {
        Server server(port);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error starting the server: " << e.what() << std::endl;
        return 1;
    }*/

    Player player(1, PlayerType::TERRORIST);

    player.setPosition(5, 10);
    std::cout << "Player in position: (" << player.getX() << ", " << player.getY() << ")" << std::endl;

    auto* weapon = new WeaponAk47();
    player.setPrimaryWeapon(weapon);
    player.setEquippedWeapon(WeaponType::PRIMARY);

    std::cout << "Ataque 1 - daño causado: " << player.attack() << std::endl;
    std::cout << "Ataque 2 - daño causado: " << player.attack() << std::endl;

    Map mapa;
    std::cout << "Mapa:\n";
    mapa.show();

    std::cout << "¿(1,1) es transitable? " << mapa.isWalkable(1, 1) << "\n";
    std::cout << "¿(0,0) es transitable? " << mapa.isWalkable(0, 0) << "\n";

    delete weapon;


    return 0;
}
