#ifndef GAME_H
#define GAME_H

#include <string>

#include "common/dtos/MatchInfo.h"
#include "utils/EventHandler.h"

#include "Graphics.h"
#include "World.h"
#include "client.h"

class Game {
private:
    Client& client;  // This is the connection with the server.
    const std::string match_name;
    Graphics graphics;
    World world;
    EventHandler eventHandler;

    bool is_running;
    void update(float dt);
    void render();

public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a game with the received configurations. All the objects of the game
    /// are created here too.
    ///
    /// \param[in] client class that has the connection with the server.
    /// \param[in] match_info structure containing the configuration for the game match.
    ////////////////////////////////////////////////////////////
    Game(Client& client, const MatchInfo& match_info, const EntitySnapshot& firstLocalPlayerSnap);

    ///
    /// \brief Initiate the game loop.
    ///
    void start();

    ~Game();
};

#endif  // GAME_H
