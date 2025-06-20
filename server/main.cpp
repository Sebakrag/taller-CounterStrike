
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "include/server.h"
#include "include/yaml_config.h"

#define PATH_CONFIG_YAML "server/config.YAML"

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    const std::string port = argv[1];


    try {
        Server server(port, PATH_CONFIG_YAML);
        server.run();

        // testeo rapido
        // std::vector<PlayerInfo> players;
        //
        // PlayerInfo p1(1, "player1", Team::CounterTerrorist, PlayerSkin::CounterTerrorist1, 100,
        // 100, Vec2D(1, 0), TypeWeapon::Secondary, 100, 0, 30); players.push_back(p1);
        //
        // std::vector<BulletInfo> bullets;
        // bullets.push_back(BulletInfo(2,Weapon::Ak47, 100, 50, Vec2D(0,1)));
        //
        // std::vector<ItemInfo> items;
        // items.push_back(ItemInfo(3,TypeItem::M3,30, 20, 20));
        //// creo un game info para probar
        // GameInfo gameInfo(GamePhase::Combat,false, 0, 0, 20.088, players, bullets, items);
        //
        // auto buffer = gameInfo.toBytes();
        //
        // std::cout << "Buffer en hexadecimal: ";
        // for (unsigned char byte : buffer) {
        //    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte)
        //    << " ";
        //}
        // std::cout << std::dec << std::endl;
        // GameInfo gameInfo2(buffer);
        // gameInfo2.print();

        // auto aux = gameInfo2.getSnapshots();

    } catch (const std::exception& e) {
        std::cerr << "Error starting the server: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
