#ifndef GAME_H
#define GAME_H

#include "../../../client/include/client.h"
#include "../../../common/dtos/MatchInfo.h"
#include "utils/EventHandler.h"

#include "Graphics.h"
#include "World.h"

class Game {
private:
    Client& client;  // This is the connection with the server.
    Graphics graphics;
    // Audio audio // Modulo encargado de ecapsular el audio del juego (Mixer, Music, etc.)
    World world;
    EventHandler eventHandler;
    GameInfo gameInfo;  // TODO: Quizas solo necesitamos guardar el GamePhase.

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
    Game(Client& client, const MatchInfo& match_info);

    ///
    /// \brief Initiate the game loop.
    ///
    void start();

    ~Game();
};

#endif  // GAME_H
