#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../../../common/dtos/FovConfig.h"
#include "../../../common/dtos/WindowConfig.h"
#include "../../../common/utils/Vec2D.h"

using SDL2pp::Color;
using SDL2pp::NullOpt;
using SDL2pp::Optional;
using SDL2pp::Point;
using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::SDLImage;
using SDL2pp::SDLTTF;
using SDL2pp::Texture;
using SDL2pp::Window;

class World;

class Graphics {
private:
    SDL sdl;
    SDLImage sdl_image;
    SDLTTF sdl_ttf;

    Window window;
    Renderer renderer;
    // Mouse mouse;  // TODO: crear un mouse con textura y renderizarlo despues del mundo.
    // TODO: puedo encapsular todo lo relacionado al audio del juego en una clase Audio.

    Window create_window(const WindowConfig& config, const std::string& match_name) const;
    Renderer create_renderer(Window& window);

public:
    Graphics(const WindowConfig& config, const FOVConfig& fov_config,
             const std::string& match_name);

    // void render(World& world);
    void clear();
    void present();
    void fillRect(const Rect& rect, const Color& color);
    void draw(Texture& tex, const Optional<Rect>& srcRect = NullOpt,
              const Optional<Rect>& dstRect = NullOpt);
    void draw(Texture& tex, const Optional<Rect>& srcRect, const Optional<Rect>& dstRect,
              double angle, const Optional<Point>& center = NullOpt, int flip = 0);

    Vec2D getDrawableWindowDimension() const;

    // agrego metodos par usar en el map
    std::shared_ptr<Texture> createTargetTexture(int w, int h);
    void setRenderTarget(Texture& tex);
    void resetRenderTarget();
    void clearWithTransparentBlack();
};

#endif  // GRAPHICS_H
