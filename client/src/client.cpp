#include "../../client/include/client.h"

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

Client::Client(const std::string& ip, const std::string& port, const std::string& user_name):
        protocol(ip.c_str(), port.c_str(), user_name),
        username(user_name),
        sender(protocol, send_queue),
        receiver(protocol, recv_queue) {
    // protocol.sendUserName(user_name);
    bool ok = protocol.recvConfirmation();
    if (!ok) {
        throw std::runtime_error(
                "No se pudo ingresar. Posiblemente el nombre de usuario ya está en uso.");
    }
    status = InMenu;
}

void Client::ExitGame() {
    if (status == InMenu) {
        protocol.sendMenuAction(MenuAction(MenuActionType::Exit));
    } else if (status == InGame) {
        receiver.kill();
        sender.kill();
        receiver.join();
        sender.join();
    }
    status = Disconnected;
}

bool Client::CreateMatch(const std::string& match_name, const std::string& scenario_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Create, match_name, scenario_name));
    bool created = protocol.recvConfirmation();
    if (created) {
        status = InLobby;
        player_creator = true;
        this->match_name = match_name;
    } else {
        std::cerr << "La partida No se pudo crear." << std::endl;
    }
    return created;
}
void Client::JoinMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Join, match_name));
    bool united = protocol.recvConfirmation();
    if (united) {
        status = InLobby;
        player_creator = false;
        this->match_name = match_name;
    } else {
        std::cerr << "No pudiste unirte a la partida." << std::endl;
    }
}
std::vector<std::string> Client::refreshMatchList() {
    protocol.sendMenuAction(MenuAction(MenuActionType::List));
    std::vector<std::string> list_matchs = protocol.recvListMatchs();
    // std::cout << "Partidas:" << std::endl;
    // for (const auto& partida: lista_partidas) {
    //     std::cout << " - " << partida << std::endl;
    // }
    return list_matchs;
}

std::vector<std::string> Client::getScenarioList() {
    protocol.sendMenuAction(MenuAction(MenuActionType::ListScenarios));
    std::vector<std::string> scenario_list = protocol.recvListScenaries();
    return scenario_list;
}

// in Lobby.
void Client::LeaveMatch() {
    try {
        protocol.sendLobbyAction(LobbyAction(LobbyAction::QuitMatch));
        bool left = protocol.recvConfirmation();
        if (left) {
            status = InMenu;
            player_creator = false;
            match_name = "";
        } else {
            std::cerr << "No pudiste abandonar la partida." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[client] ERROR: error leaving the match" << e.what() << std::endl;
        status = InMenu;
        player_creator = false;
        match_name = "";
    }
}
void Client::StartMatch() {
    protocol.sendLobbyAction(LobbyAction::StartMatch);
    bool ok = protocol.recvConfirmation();
    if (ok) {
        startThreads();
    } else {
        std::cerr << "No empezó la partida. No sos el anfitrion." << std::endl;
    }
}

std::vector<PlayerInfoLobby> Client::refreshPlayersList() {
    if (status != InLobby) {
        throw std::runtime_error(
                "No puedes realizar Client::refreshPlayersList() si no estás en el lobby");
    }
    protocol.sendLobbyAction(LobbyAction::ListPlayers);

    MatchRoomInfo info = protocol.recvUpdateMatchRoom();
    if (info.matchStarted) {
        startThreads();
    }
    return info.players;
}

MatchInfo Client::getMatchInfo() {
    if (status != InGame) {
        throw std::runtime_error("Error. El matchInfo aún no fue recibido del servidor.");
    }
    return matchInfo;
}


GameInfo Client::getGameInfo() {
    GameInfo g = recv_queue.pop();
    // g.print();
    return g;
}

std::optional<GameInfo> Client::tryGetGameInfo() {
    GameInfo g;
    // o pop(). Tener en cuenta si la interfaz debe esperar o seguir igual
    if (recv_queue.try_pop(g))
        return g;
    return std::nullopt;
}

void Client::move(const Vec2D& direction) {
    send_queue.try_push(GameAction(GameActionType::Walk, direction));
}

void Client::shoot(const AimInfo& aimInfo) {
    send_queue.try_push(GameAction(GameActionType::Attack, aimInfo.direction));
}

void Client::rotate(const float angle) {
    send_queue.try_push(GameAction(GameActionType::Rotate, angle));
}
void Client::changeWeapon(const TypeWeapon& typeWeapon) {
    send_queue.try_push(GameAction(GameActionType::ChangeWeapon, typeWeapon));
}

void Client::pickUpItem() { send_queue.try_push(GameAction(GameActionType::PickUp)); }

void Client::buyWeapon(const Weapon weapon) {
    send_queue.try_push(GameAction(GameActionType::BuyWeapon, weapon));
}

void Client::buyAmmo(const AmmoType ammoType) {
    send_queue.try_push(GameAction(GameActionType::BuyAmmo, ammoType, 10));
}

void Client::defuseBomb() { send_queue.try_push(GameAction(GameActionType::DefuseBomb)); }

Client::~Client() {}

Status Client::getStatus() { return status; }

// metodos privados:
void Client::startThreads() {
    status = InGame;
    matchInfo = protocol.recvMatchInfo();
    sender.start();
    receiver.start();
}
