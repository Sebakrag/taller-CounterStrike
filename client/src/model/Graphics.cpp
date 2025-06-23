#include "../../../client/include/model/Graphics.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>

#include "../../../client/client_constants.h"
#include "../../../client/include/model/World.h"
#include "../../../client/include/model/utils/DynamicStencil.h"
#include "../../../client/include/model/utils/TextureManager.h"

#define GAME_NAME "Counter Strike"

Graphics::Graphics(const WindowConfig& config, const FOVConfig& fov_config,
                   const std::string& match_name):
        sdl(SDL_INIT_VIDEO || SDL_INIT_AUDIO),
        sdl_image(IMG_INIT_PNG | IMG_INIT_JPG),
        sdl_ttf(),
        window(create_window(config, match_name)),
        renderer(create_renderer(window)) {
    TextureManager::init(renderer);
    DynamicStencil::init(renderer, fov_config);
}

Window Graphics::create_window(const WindowConfig& config, const std::string& match_name) const {
    const std::string win_title = std::string(GAME_NAME) + " - " + match_name;
    return Window(win_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.width,
                  config.height, config.flags);
}

Renderer Graphics::create_renderer(Window& window) {
    Renderer ren(window, -1, SDL_RENDERER_ACCELERATED);
    ren.SetDrawColor(0, 0, 0, 255);
    return ren;
}

// void Graphics::render(World& world) {
//     renderer.Clear();
//     world.render();
//     // mouse.render();  // TODO: crear un mouse.
//     renderer.Present();
// }

void Graphics::clear() { renderer.Clear(); }

void Graphics::present() { renderer.Present(); }

void Graphics::fillRect(const Rect& rect, const Color& color) {
    renderer.SetDrawColor(color);
    renderer.FillRect(rect);
}

void Graphics::draw(Texture& tex, const Optional<Rect>& srcRect, const Optional<Rect>& dstRect) {
    renderer.Copy(tex, srcRect, dstRect);
}

void Graphics::draw(Texture& tex, const Optional<Rect>& srcRect, const Optional<Rect>& dstRect,
                    const double angle, const Optional<Point>& center, const int flip) {
    renderer.Copy(tex, srcRect, dstRect, angle, center, flip);
}

Vec2D Graphics::getDrawableWindowDimension() const {
    const Point dimension = window.GetDrawableSize();
    return {static_cast<float>(dimension.GetX()), static_cast<float>(dimension.GetY())};
}

std::shared_ptr<Texture> Graphics::createTargetTexture(int width, int height) {
    return std::make_shared<Texture>(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                     width, height);
}

void Graphics::setRenderTarget(Texture& tex) { renderer.SetTarget(tex); }

void Graphics::resetRenderTarget() { renderer.SetTarget(); }

void Graphics::clearWithTransparentBlack() {
    renderer.SetDrawColor(0, 0, 0, 0);
    renderer.Clear();
}
