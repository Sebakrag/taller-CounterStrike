#include "client/include/model/utils/InputHandler.h"

#include <iostream>

#include "client/include/model/EC/components/InputComponent.h"

InputHandler::InputHandler(const Entity local_player): local_player(local_player) {}

void InputHandler::handle_event(const SDL_Event& e, ComponentManager& cm) {
    try {
        auto* input = cm.getComponent<InputComponent>(local_player);
        if (input == nullptr)
            return;

        switch (e.type) {
            case SDL_KEYDOWN: {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        input->move_dir += Vec2D(0.0f, -1.0f);
                        break;
                    case SDLK_s:
                        input->move_dir += Vec2D(0.0f, 1.0f);
                        break;
                    case SDLK_a:
                        input->move_dir += Vec2D(-1.0f, 0.0f);
                        break;
                    case SDLK_d:
                        input->move_dir += Vec2D(1.0f, 0.0f);
                        break;
                    case SDLK_q:
                        input->switch_weapon = true;
                        break;
                    default:
                        break;
                }
                break;
            }  // end (SDL_KEYDOWN)
            case SDL_KEYUP:
                break;

            default:
                break;
        }

        std::cout << "InputSytem updated!!\n" << std::endl;
    } catch (const std::exception& exp) {
        std::cout << exp.what() << std::endl;
    }
}
