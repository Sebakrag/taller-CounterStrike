#include "client/include/model/Graphics.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "client/include/model/World.h"
#include "client/include/model/utils/TextureManager.h"

#define GAME_NAME "Counter Strike"

Graphics::Graphics(const WindowConfig& config, const std::string& match_name):
        sdl(SDL_INIT_VIDEO),
        sdl_image(IMG_INIT_PNG | IMG_INIT_JPG),
        sdl_ttf(),
        window(create_window(config, match_name)),
        renderer(create_renderer(window)) {
    TextureManager::init(renderer);
}

Window Graphics::create_window(const WindowConfig& config, const std::string& match_name) const {
    const std::string win_title = std::string(GAME_NAME) + " - " + match_name;
    return Window(win_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width,
                  config.height, config.flags);
}

Renderer Graphics::create_renderer(Window& window) {
    Renderer ren(window, -1, SDL_RENDERER_ACCELERATED);
    ren.SetDrawColor(255, 255, 255, 255);
    return ren;
}

void Graphics::render(World& world) {
    renderer.Clear();
    world.render(*this);
    renderer.Present();
}

void Graphics::draw(Texture& tex, const Optional<Rect>& srcRect, const Optional<Rect>& dstRect) {
    renderer.Copy(tex, srcRect, dstRect);
}

void Graphics::draw(Texture& tex, const Optional<Rect>& srcRect, const Optional<Rect>& dstRect,
                    const double angle, const Optional<Point>& center, const int flip) {
    renderer.Copy(tex, srcRect, dstRect, angle, center, flip);
}
