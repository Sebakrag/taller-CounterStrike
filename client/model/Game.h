#ifndef GAME_H
#define GAME_H

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "client/dtos/matchInfo.h"

#include "Map.h"

#define GAME_NAME "Counter Strike"

using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::SDLImage;
using SDL2pp::SDLTTF;
using SDL2pp::Window;

class Game {
private:
    const std::string match_name;

    SDL sdl;
    SDLImage sdl_image;
    SDLTTF sdl_ttf;

    Window window;
    Renderer renderer;
    Map map;

    bool is_running;

    Window create_window(const match_info_t& match_info) const;
    Renderer create_renderer(Window& window);

public:
    ////////////////////////////////////////////////////////////
    /// \brief Create a game with the received configurations. All the objects of the game
    /// are created here too.
    ///
    /// \param[in] match_info structure containing the configuration for the game match.
    /// \param[in] img_flags Flags to pass to IMG_Init()
    ////////////////////////////////////////////////////////////
    Game(const match_info_t& match_info, const uint32_t img_flags);

    void handle_events();
    void update();
    void render();
    bool running() const;

    ~Game();
};

#endif  // GAME_H
