#include "../include/client.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "../include/client_protocol.h"

Client::Client(const EntitySnapshot& snap):
        snap(snap), x(snap.pos_x), y(snap.pos_y), angle(snap.angle) {}

// Client::Client(const std::string& ip, const std::string& port, const std::string& user_name):
//         protocol(ip.c_str(), port.c_str(), user_name),
//         sender(protocol, send_queue),
//         receiver(protocol, recv_queue) {
//     protocol.sendUserName(user_name);
//     bool ok = protocol.recvConfirmation();
//     if (!ok) {
//         throw std::runtime_error(
//                 "No se pudo ingresar. Posiblemente el nombre de usuario ya está en uso.");
//     }
//     std::cout << "Bienvenido " << user_name << ". Ya estás conectado al servidor." << std::endl;
//     status = InMenu;
//     // recvDatos();
//
//     CreateMatch("Partidita");
//     StartMatch();
// }
// void Client::mainLoop() {
//     while (status != Disconnected) {
//         if (status == InMenu) {
//             menuLoop();
//         }
//         if (status == InLobby) {
//             lobbyLoop();
//         }
//         if (status == InGame) {
//             sender.join();
//             receiver.join();
//         }
//     }
// }
// // Para probar (por ahora), voy a leer de la terminal el stdin
// void Client::menuLoop() {
//     std::cout << "Estás en el Menú" << std::endl;
//     std::cout << "Podes escribir: 'crear <nombre>', 'unirse <nombre> ', 'listar' o  'salir'"
//               << std::endl;
//     std::string msj_usuario;
//     while (status == InMenu) {
//         std::getline(std::cin, msj_usuario);
//         if (msj_usuario.substr(0, 6) == "crear ") {
//             std::string nombre_partida = msj_usuario.substr(6, msj_usuario.length());
//             CreateMatch(nombre_partida);
//         } else if (msj_usuario.substr(0, 7) == "unirse ") {
//             std::string nombre_partida = msj_usuario.substr(7, msj_usuario.length());
//             JoinMatch(nombre_partida);
//         } else if (msj_usuario == "listar") {
//             refreshMatchList();
//         } else if (msj_usuario == "salir") {
//             ExitGame();
//         }
//     }
// }
// // Para probar (por ahora), voy a leer de la terminal el stdin
// void Client::lobbyLoop() {
//     std::cout << "Estás en el Lobby" << std::endl;
//     std::cout << "Podes escribir: 'actualizar', 'abandonar', 'iniciar'" << std::endl;
//     std::string msj_usuario;
//     while (status == InLobby) {
//         std::getline(std::cin, msj_usuario);
//         if (msj_usuario == "actualizar") {
//             refreshMatchRoom();
//         } else if (msj_usuario == "abandonar") {
//             LeaveMatch();
//         } else if (msj_usuario == "iniciar") {
//             StartMatch();
//         }
//     }
// }
// void Client::ExitGame() {
//     status = Disconnected;
//     protocol.sendMenuAction(MenuAction(MenuActionType::Exit));
// }
// void Client::CreateMatch(const std::string& match_name) {
//     protocol.sendMenuAction(MenuAction(MenuActionType::Create, match_name, 0));
//     bool created = protocol.recvConfirmation();
//     if (created) {
//         std::cout << "La partida se creó correctamente." << std::endl;
//         status = InLobby;
//         // protocol.recvTileMap
//     } else {
//         std::cout << "La partida No se pudo crear." << std::endl;
//     }
// }
// void Client::JoinMatch(const std::string& match_name) {
//     protocol.sendMenuAction(MenuAction(MenuActionType::Join, match_name));
//     bool united = protocol.recvConfirmation();
//     if (united) {
//         std::cout << "Te uniste a la partida!" << std::endl;
//         status = InLobby;
//         // protocol.recvTileMap
//     } else {
//         std::cout << "No pudiste unirte a la partida." << std::endl;
//     }
// }
// void Client::refreshMatchList() {
//     protocol.sendMenuAction(MenuAction(MenuActionType::List));
//     std::vector<std::string> lista_partidas = protocol.recvListMatchs();
//     std::cout << "Partidas:" << std::endl;
//     for (const auto& partida: lista_partidas) {
//         std::cout << " - " << partida << std::endl;
//     }
// }
// // in Lobby.
// void Client::LeaveMatch() {
//     protocol.sendLobbyAction(LobbyAction::QuitMatch);
//     status = InMenu;
// }
// void Client::StartMatch() {
//     protocol.sendLobbyAction(LobbyAction::StartMatch);
//     bool ok = protocol.recvConfirmation();
//     if (ok) {
//         std::cout << "Empezó la partida" << std::endl;
//         status = InGame;
//         sender.start();
//         receiver.start();
//     } else {
//         std::cout << "No empezó la partida. No sos el anfitrion." << std::endl;
//     }
// }
//
// void Client::refreshMatchRoom() {
//     protocol.sendLobbyAction(LobbyAction::ListPlayers);
//     MatchRoomInfo info = protocol.recvUpdateMatchRoom();
//     std::cout << "Players en la sala: " << std::endl;
//     for (const auto& p: info.players) {
//         std::cout << " - " << p.username << std::endl;
//     }
//     if (info.matchStarted) {
//         status = InGame;
//         sender.start();
//         receiver.start();
//     }
// }

// std::vector<EntitySnapshot> Client::getGameInfo() {
//     // const EntitySnapshot s = {snap.server_entt_id, this->x,        this->y, this->angle,
//     //                           snap.sprite_type,    snap.entt_type, snap.hp, snap.money,
//     //                           snap.is_alive};
//     // std::vector<EntitySnapshot> v(1, s);
//     //
//     // return v;
//     GameInfo g = recv_queue.pop();
//     std::cout << "Cantidad de snapshots: " << g.getSnapshots().size() << std::endl;
//
//
//     return g.getSnapshots();
// }
//
// GameInfo Client::getGameInfo2() {
//     GameInfo g;
//     recv_queue.try_pop(g);  // o pop(). Tener en cuenta si la interfaz debe esperar o seguir
//     igual return g;
// }
//
// void Client::move(const Vec2D& direction) {
//     send_queue.try_push(GameAction(GameActionType::Walk, direction));
// }
//
// void Client::shoot(const AimInfo& aimInfo) {
//     send_queue.try_push(GameAction(GameActionType::Attack, aimInfo.direction));
// }

// proximamente :) TODO: Implementar esto
void Client::rotate(const float angle) { this->angle = angle; }

std::vector<EntitySnapshot> Client::getGameInfo() {
    std::vector<EntitySnapshot> v;
    if (const auto player = std::get_if<PlayerSnapshot>(&snap.data)) {
        const EntitySnapshot s(snap.server_entt_id, snap.type, snap.sprite_type, this->x, this->y,
                               this->angle, snap.is_alive, player->hp, player->money, player->ammo,
                               player->state, player->equipped_weapon_id, player->team);
        v.emplace_back(s);
    }

    const EntitySnapshot s2(9, EntityType::WEAPON, SpriteType::AWP, 30025.55, 30000, 0, true,
                            WeaponState::DROPPED);
    v.emplace_back(s2);
    const EntitySnapshot s3(6, EntityType::WEAPON, SpriteType::GLOCK, 30025.55, 30000, 0, true,
                            WeaponState::EQUIPPED);
    v.emplace_back(s3);

    return v;
}

void Client::move(const Vec2D& direction) {
    this->x += direction.getX() * 5;
    this->y += direction.getY() * 5;
}

void Client::shoot(const AimInfo& aimInfo) { this->angle = aimInfo.angle; }

Client::~Client() {}

// Status Client::getStatus() { return status; }
