#ifndef GAME_H
#define GAME_H

#include <string>

#include "client/Client.h"
#include "client/dtos/matchInfo.h"
#include "utils/EventHandler.h"

#include "Graphics.h"
#include "World.h"

class Game {
private:
    Client& client;  // This is the connection with the server.
    const std::string match_name;
    Graphics graphics;
    World world;
    EventHandler eventHandler;

    bool is_running;
    void handleServerMessages();
    void update(float dt);
    void render();

public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a game with the received configurations. All the objects of the game
    /// are created here too.
    ///
    /// \param[in] match_info structure containing the configuration for the game match.
    /// \param[in] client class that has the connection with the server.
    ////////////////////////////////////////////////////////////
    Game(const match_info_t& match_info, Client& client);

    ///
    /// \brief Initiate the game loop.
    ///
    void start();

    ~Game();
};

#endif  // GAME_H
