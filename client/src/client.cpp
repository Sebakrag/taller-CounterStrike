#include "../include/client.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "../include/client_protocol.h"

// Client::Client(const EntitySnapshot& snap):
//       snap(snap), x(snap.pos_x), y(snap.pos_y), angle(snap.angle) {}

Client::Client(const std::string& ip, const std::string& port, const std::string& user_name):
        protocol(ip.c_str(), port.c_str(), user_name),
        sender(protocol, send_queue),
        receiver(protocol, recv_queue) {
    protocol.sendUserName(user_name);
    bool ok = protocol.recvConfirmation();
    if (!ok) {
        throw std::runtime_error(
                "No se pudo ingresar. Posiblemente el nombre de usuario ya está en uso.");
    }
    std::cout << "Bienvenido " << user_name << ". Ya estás conectado al servidor." << std::endl;
    status = InMenu;
}

void Client::ExitGame() {
    status = Disconnected;
    protocol.sendMenuAction(MenuAction(MenuActionType::Exit));
}
void Client::CreateMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Create, match_name, 0));
    bool created = protocol.recvConfirmation();
    if (created) {
        std::cout << "La partida se creó correctamente." << std::endl;
        status = InLobby;
        matchInfo = protocol.recvMatchInfo();
    } else {
        std::cout << "La partida No se pudo crear." << std::endl;
    }
}
void Client::JoinMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Join, match_name));
    bool united = protocol.recvConfirmation();
    if (united) {
        std::cout << "Te uniste a la partida!" << std::endl;
        status = InLobby;
        matchInfo = protocol.recvMatchInfo();
    } else {
        std::cout << "No pudiste unirte a la partida." << std::endl;
    }
}
void Client::refreshMatchList() {
    protocol.sendMenuAction(MenuAction(MenuActionType::List));
    std::vector<std::string> lista_partidas = protocol.recvListMatchs();
    std::cout << "Partidas:" << std::endl;
    for (const auto& partida: lista_partidas) {
        std::cout << " - " << partida << std::endl;
    }
}

// in Lobby.
void Client::LeaveMatch() {
    protocol.sendLobbyAction(LobbyAction::QuitMatch);
    status = InMenu;
}
void Client::StartMatch() {
    protocol.sendLobbyAction(LobbyAction::StartMatch);
    bool ok = protocol.recvConfirmation();
    if (ok) {
        std::cout << "Empezó la partida" << std::endl;
        status = InGame;
        sender.start();
        receiver.start();
    } else {
        std::cout << "No empezó la partida. No sos el anfitrion." << std::endl;
    }
}

void Client::refreshMatchRoom() {
    protocol.sendLobbyAction(LobbyAction::ListPlayers);
    MatchRoomInfo info = protocol.recvUpdateMatchRoom();
    std::cout << "Players en la sala: " << std::endl;
    for (const auto& p: info.players) {
        std::cout << " - " << p.username << std::endl;
    }
    if (info.matchStarted) {
        status = InGame;
        sender.start();
        receiver.start();
    }
}

MatchInfo Client::getMatchInfo() {
    if (status == Status::Disconnected || status == Status::InMenu) {
        throw std::runtime_error("Error. El matchInfo aún no fue recibido del servidor.");
    }
    return matchInfo;
}


std::vector<EntitySnapshot> Client::getGameInfo() {
    // const EntitySnapshot s = {snap.server_entt_id, this->x,        this->y, this->angle,
    //                           snap.sprite_type,    snap.entt_type, snap.hp, snap.money,
    //                           snap.is_alive};
    // std::vector<EntitySnapshot> v(1, s);
    //
    // return v;
    GameInfo g = recv_queue.pop();
    // std::cout << "Cantidad de snapshots: " << g.getSnapshots().size() << std::endl;


    return g.getSnapshots();
}

GameInfo Client::getGameInfo2() {
    GameInfo g;
    recv_queue.try_pop(g);  // o pop(). Tener en cuenta si la interfaz debe esperar o seguir igual
    return g;
}

void Client::move(const Vec2D& direction) {
    send_queue.try_push(GameAction(GameActionType::Walk, direction));
}

void Client::shoot(const AimInfo& aimInfo) {
    send_queue.try_push(GameAction(GameActionType::Attack, aimInfo.direction));
}

// proximamente :) TODO: Implementar esto
void Client::rotate(const float angle) {
    send_queue.try_push(GameAction(GameActionType::Rotate, angle));
}

Client::~Client() {}

Status Client::getStatus() { return status; }
