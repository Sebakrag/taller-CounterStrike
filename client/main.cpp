#include <exception>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL_image.h>

#include "common/constants.h"

#include "connection_info.h"

using SDL2pp::Color;
using SDL2pp::Font;
using SDL2pp::Point;
using SDL2pp::Rect;
using SDL2pp::Renderer;
using SDL2pp::SDL;
using SDL2pp::SDLImage;
using SDL2pp::SDLTTF;
using SDL2pp::Surface;
using SDL2pp::Texture;
using SDL2pp::Window;
using std::nullopt;

connection_info_t start_login();

int main() {
    try {
        connection_info_t conn_info = start_login();

        std::cout << conn_info.get_hostname() << std::endl;
        // Client client(conn_info.get_hostname(), conn_info.get_port(), conn_info.get_usr_name());
        // client.start();

        // Here all resources are automatically released and library deinitialized
        return SUCCESS;
    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return ERROR;
    }
}

connection_info_t start_login() {
    SDL sdl(SDL_INIT_VIDEO);
    SDLImage sdlImage(IMG_INIT_PNG);
    SDLTTF sdlTtf;

    Window window("User Login", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH,
                  WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
    Texture background(renderer, "client/assets/login_background.png");

    Font font("client/assets/OpenSans-Light.ttf", 24);
    Color white = {255, 255, 255, 255};
    Surface textSurface = font.RenderText_Blended("Argentinian Server", white);
    Texture textTexture(renderer, textSurface);

    std::string hostname;
    std::string port;

    bool quit = false;
    SDL_Event e;

    Point p = window.GetSize();
    int buttonWidth = p.GetX() * 0.25;
    int buttonHeight = p.GetY() * 0.1;
    int buttonX = (p.GetX() - buttonWidth) / 2;
    int buttonY = p.GetY() - buttonHeight - 50;
    Rect buttonRect(buttonX, buttonY, buttonWidth, buttonHeight);

    Point point = textTexture.GetSize();
    Rect textRect = {buttonRect.GetX() + (buttonRect.GetW() - point.GetX()) / 2,
                     buttonRect.GetY() + (buttonRect.GetH() - point.GetY()) / 2, point.GetX(),
                     point.GetY()};

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                        quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (buttonRect.Contains(e.button.x, e.button.y)) {
                        std::cout << "Botón 'Argentinian Server' presionado." << std::endl;
                        hostname = SERVER_ADDRESS;
                        port = SERVER_PORT;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                        p = window.GetSize();
                        buttonWidth = p.GetX() * 0.25;
                        buttonHeight = p.GetY() * 0.1;
                        buttonX = (p.GetX() - buttonWidth) / 2;
                        buttonY = p.GetY() - buttonHeight - 50;
                        buttonRect.SetX(buttonX);
                        buttonRect.SetY(buttonY);
                        buttonRect.SetW(buttonWidth);
                        buttonRect.SetH(buttonHeight);

                        textRect.SetX(buttonRect.GetX() + (buttonRect.GetW() - point.GetX()) / 2);
                        textRect.SetY(buttonRect.GetY() + (buttonRect.GetH() - point.GetY()) / 2);
                    }
                    break;
            }
        }

        renderer.Clear();
        renderer.Copy(background);

        renderer.SetDrawColor(255, 255, 255, 255);
        renderer.FillRect(buttonRect);

        renderer.Copy(textTexture, nullopt, textRect);

        renderer.Present();
    }

    return connection_info_t(hostname, port, "jugador");
}


/*
connection_info_t start_login() {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDLImage image(IMG_INIT_PNG);
    //SDL_ENABLE

    // Create login window
    Window window("User Login", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                  SDL_WINDOW_RESIZABLE);

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Texture background(renderer, Surface("login_background.png"));

    // Clear screen
    renderer.Clear();

    renderer.Copy(background);

    // Show rendered frame
    renderer.Present();

    // 5 second delay
    SDL_Delay(5000);
    // Here all resources are automatically released and library deinitialized
    return connection_info_t(hostname, port, usr_name);
}
*/

/*
connection_info_t start_login() {
    SDL sdl(SDL_INIT_VIDEO);
    SDLImage sdl_image(IMG_INIT_PNG);
    SDLTTF sdl_ttf;

    Window window("Login", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
SDL_WINDOW_SHOWN); Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Carga de imagen de fondo
    // Surface bgSurface("assets/login_background.png");
    // Texture background(renderer, bgSurface);
    Texture background(renderer, "assets/login_background.png");

    // Fuente para el texto
    Font font("assets/OpenSans-Regular.ttf", 24);

    std::string username;
    bool inputActive = true;

    std::string hostname = SERVER_ADDRESS;
    std::string port = SERVER_PORT;

    Rect buttonRect(300, 400, 200, 50);

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_TEXTINPUT && inputActive) {
                username += e.text.text;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && !username.empty()) {
                    username.pop_back();
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                if (x >= buttonRect.GetX() && x <= buttonRect.GetX() + buttonRect.GetW() &&
                    y >= buttonRect.GetY() && y <= buttonRect.GetY() + buttonRect.GetH()) {
                    std::cout << "Botón presionado. Username: " << username << std::endl;
                    // Acción personalizada
                }
            }
        }
    }
    renderer.Clear();

    // Fondo
    renderer.Copy(background);

    // Cuadro de texto
    std::string inputText = username.empty() ? "Enter username..." : username;
    Texture inputTexture(renderer, font.RenderText_Blended(inputText, Color(255, 255, 255)));
    renderer.Copy(inputTexture, NullOpt, Rect(200, 250, inputTexture.GetWidth(),
inputTexture.GetHeight()));

    // Botón
    SDL_SetRenderDrawColor(renderer.Get(), 0, 123, 255, 255);
    SDL_Rect rawButton = buttonRect;
    SDL_RenderFillRect(renderer.Get(), &rawButton);

    Texture buttonText(renderer, font.RenderText_Blended("Argentinian Server", Color(255, 255,
255))); int textX = buttonRect.GetX() + (buttonRect.GetW() - buttonText.GetWidth()) / 2; int textY =
buttonRect.GetY() + (buttonRect.GetH() - buttonText.GetHeight()) / 2; renderer.Copy(buttonText,
NullOpt, Rect(textX, textY, buttonText.GetWidth(), buttonText.GetHeight()));

    renderer.Present();

    SDL_Delay(30000);

    return connection_info_t(hostname, port, username);
}
*/
