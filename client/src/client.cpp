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
        username(user_name),
        sender(protocol, send_queue),
        receiver(protocol, recv_queue),
        // Inicializar snap con valores válidos para PLAYER (usando el constructor completo para PLAYER)
        // Parámetros adicionales: hp=100, money=1000, ammo=30, estado=Idle, weapon_id=0, team=CounterTerrorist
        snap(1, EntityType::PLAYER, SpriteType::PHEONIX, 0.0f, 0.0f, 0.0f, true, 100, 1000, 30, PlayerState::Idle, 0, Team::CounterTerrorist),
        x(0.0f),
        y(0.0f),
        angle(0.0f) {
    protocol.sendUserName(user_name);
    bool ok = protocol.recvConfirmation();
    if (!ok) {
        throw std::runtime_error(
                "No se pudo ingresar. Posiblemente el nombre de usuario ya está en uso.");
    }
    std::cout << "Bienvenido " << user_name << ". Ya estás conectado al servidor." << std::endl;
    status = InMenu;


    // CreateMatch("Partidita");
    // StartMatch();
}
void Client::mainLoop() {
    while (status != Disconnected) {
        if (status == InMenu) {
            menuLoop();
        }
        if (status == InLobby) {
            lobbyLoop();
        }
        if (status == InGame) {
            sender.join();
            receiver.join();
        }
    }
}
// Para probar (por ahora), voy a leer de la terminal el stdin
void Client::menuLoop() {
    std::cout << "Estás en el Menú" << std::endl;
    std::cout << "Podes escribir: 'crear <nombre>', 'unirse <nombre> ', 'listar' o  'salir'"
              << std::endl;
    std::string msj_usuario;
    while (status == InMenu) {
        std::getline(std::cin, msj_usuario);
        if (msj_usuario.substr(0, 6) == "crear ") {
            std::string nombre_partida = msj_usuario.substr(6, msj_usuario.length());
            CreateMatch(nombre_partida);
        } else if (msj_usuario.substr(0, 7) == "unirse ") {
            std::string nombre_partida = msj_usuario.substr(7, msj_usuario.length());
            JoinMatch(nombre_partida);
        } else if (msj_usuario == "listar") {
            refreshMatchList();
        } else if (msj_usuario == "salir") {
            ExitGame();
        }
    }
}
// Para probar (por ahora), voy a leer de la terminal el stdin
void Client::lobbyLoop() {
    std::cout << "Estás en el Lobby" << std::endl;
    std::cout << "Podes escribir: 'actualizar', 'abandonar', 'iniciar'" << std::endl;
    std::string msj_usuario;
    while (status == InLobby) {
        std::getline(std::cin, msj_usuario);
        if (msj_usuario == "actualizar") {
            refreshMatchRoom();
        } else if (msj_usuario == "abandonar") {
            LeaveMatch();
        } else if (msj_usuario == "iniciar") {
            StartMatch();
        }
    }
}
void Client::ExitGame() {
    status = Disconnected;
    protocol.sendMenuAction(MenuAction(MenuActionType::Exit));
}
#include "client/include/ui/MapSelectionDialog.h"
#include <QApplication>

bool Client::CreateMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Create, match_name, 0));
    
    // Primero recibimos la lista de mapas disponibles
    std::vector<ClientMapInfo> availableMaps;
    try {
        availableMaps = protocol.recvAvailableMaps();
        std::cout << "Mapas disponibles recibidos: " << availableMaps.size() << std::endl;
        
        // Verificar que tengamos mapas disponibles
        if (availableMaps.empty()) {
            std::cerr << "No hay mapas disponibles para crear una partida" << std::endl;
            return false;
        }
        
        // Mostrar diálogo de selección de mapas
        std::string selectedMapFile;
        
        // Necesitamos usar QApplication::instance para crear diálogos desde una clase no-Qt
        if (QApplication::instance()) {
            MapSelectionDialog mapDialog(availableMaps);
            
            // Si el usuario cancela la selección, abortamos la creación de partida
            if (mapDialog.exec() != QDialog::Accepted) {
                std::cout << "Usuario canceló la selección de mapa" << std::endl;
                return false;
            }
            
            // Obtener el mapa seleccionado
            selectedMapFile = mapDialog.selectedMapFileName().toStdString();
            std::cout << "Mapa seleccionado: " << selectedMapFile << std::endl;
        } else {
            // Fallback si no hay interfaz gráfica disponible
            selectedMapFile = availableMaps[0].fileName;
            std::cout << "Interfaz gráfica no disponible, seleccionando primer mapa: " << selectedMapFile << std::endl;
        }
        
        // Enviamos el nombre del mapa seleccionado al servidor
        protocol.sendMenuAction(MenuAction(MenuActionType::Create, match_name, 0, selectedMapFile));
        
        // Ahora esperamos la confirmación
        bool created = protocol.recvConfirmation();
        if (created) {
            std::cout << "La partida se creó correctamente." << std::endl;
            status = InLobby;
            player_creator = true;
            this->match_name = match_name;
            
            // Esperamos un momento antes de solicitar la lista de jugadores para evitar problemas de protocolo
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            // Luego solicitamos la lista de jugadores
            try {
                refreshMatchRoom();
            } catch(const std::exception& e) {
                std::cerr << "Error al actualizar la sala: " << e.what() << std::endl;
                // No fallamos toda la operación si esto falla, ya estamos en el lobby
            }
        } else {
            std::cout << "La partida No se pudo crear." << std::endl;
        }
        return created;
    } catch(const std::exception& e) {
        std::cerr << "Error al crear partida: " << e.what() << std::endl;
        return false;
    }
}
void Client::JoinMatch(const std::string& match_name) {
    protocol.sendMenuAction(MenuAction(MenuActionType::Join, match_name));
    bool united = protocol.recvConfirmation();
    if (united) {
        std::cout << "Te uniste a la partida!" << std::endl;
        status = InLobby;
        player_creator = false;
        this->match_name = match_name;
        // protocol.recvTileMap
    } else {
        std::cout << "No pudiste unirte a la partida." << std::endl;
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
// in Lobby.
void Client::LeaveMatch() {
    protocol.sendLobbyAction(LobbyAction(LobbyAction::QuitMatch));
    bool left = protocol.recvConfirmation();
    if (left) {
        std::cout << "Abandonaste la partida." << std::endl;
        status = InMenu;
        player_creator = false;
        match_name = "";
    } else {
        std::cout << "No pudiste abandonar la partida." << std::endl;
    }
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

std::vector<PlayerInfoLobby> Client::refreshMatchRoom() {
    protocol.sendLobbyAction(LobbyAction::ListPlayers);
    MatchRoomInfo info = protocol.recvUpdateMatchRoom();
    // std::cout << "Players en la sala: " << std::endl;
    // for (const auto& p: info.players) {
    //     std::cout << " - " << p.username << std::endl;
    // }
    if (info.matchStarted) {
        status = InGame;
        sender.start();
        receiver.start();
    }
    return info.players;
}

std::vector<PlayerInfoLobby> Client::refreshPlayersList() {
    protocol.sendLobbyAction(LobbyAction::ListPlayers);
    MatchRoomInfo info = protocol.recvUpdateMatchRoom();

    // std::cout << "Players en la sala: " << std::endl;
    // for (const auto& p: info.players) {
    //     std::cout << " - " << p.username << std::endl;
    // }

    if (info.matchStarted) {
        status = InGame;
        sender.start();
        receiver.start();
    }
    return info.players;
}

std::vector<EntitySnapshot> Client::getGameInfo() {
    // const EntitySnapshot s = {snap.server_entt_id, this->x,        this->y, this->angle,
    //                           snap.sprite_type,    snap.entt_type, snap.hp, snap.money,
    //                           snap.is_alive};
    // std::vector<EntitySnapshot> v(1, s);
    //
    // return v;
    GameInfo g = recv_queue.pop();
    std::cout << "Cantidad de snapshots: " << g.getSnapshots().size() << std::endl;


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
void Client::rotate(const float angle) { this->angle = angle; }

Client::~Client() {}

Status Client::getStatus() { return status; }
