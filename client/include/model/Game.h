#ifndef GAME_H
#define GAME_H

#include <string>

#include "client/dtos/matchInfo.h"

#include "Graphics.h"
#include "Map.h"
#include "World.h"

class Game {
private:
    // Client& client;   // This is the connection with the server.
    const std::string match_name;
    Graphics graphics;
    // Map map;
    World world;

    bool is_running;
    void handle_events();
    void update(float dt);
    void render();

public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a game with the received configurations. All the objects of the game
    /// are created here too.
    ///
    /// \param[in] match_info structure containing the configuration for the game match.
    ////////////////////////////////////////////////////////////
    explicit Game(const match_info_t& match_info);

    // Game(const match_info_t& match_info, Client& client);

    void game_loop();

    ~Game();
};

#endif  // GAME_H
