#include "InputSystem.h"

#include <iostream>

#include "client/model/ECS/components/InputComponent.h"

InputSystem::InputSystem(const Entity local_player): local_player(local_player) {}

void InputSystem::handle_event(const SDL_Event& e, ComponentManager& cm) {
    try {
        InputComponent* input = cm.getComponent<InputComponent>(local_player);
        if (input == nullptr)
            return;

        // Reset inputs
        input->shooting = false;
        input->switch_weapon = false;
        // input->move_dir = {0.0f, 0.0f};
        input->x = 0.0f;
        input->y = 0.0f;

        bool is_pressed;
        switch (e.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                is_pressed = (e.type == SDL_KEYDOWN);
                switch (e.key.keysym.sym) {
                    // case SDLK_w: input->move_dir.y += is_pressed ? -1.0f : 1.0f; break;
                    // case SDLK_s: input->move_dir.y += is_pressed ?  1.0f : -1.0f; break;
                    // case SDLK_a: input->move_dir.x += is_pressed ? -1.0f : 1.0f; break;
                    // case SDLK_d: input->move_dir.x += is_pressed ?  1.0f : -1.0f; break;
                    case SDLK_w:
                        input->y += is_pressed ? -1.0f : 1.0f;
                        break;
                    case SDLK_s:
                        input->y += is_pressed ? 1.0f : -1.0f;
                        break;
                    case SDLK_a:
                        input->x += is_pressed ? -1.0f : 1.0f;
                        break;
                    case SDLK_d:
                        input->x += is_pressed ? 1.0f : -1.0f;
                        break;
                    case SDLK_q:
                        input->switch_weapon = is_pressed;
                        break;

                    default:
                        break;
                }
                break;

            default:
                break;
        }


        std::cout << "InputSytem updated!!\n" << std::endl;
    } catch (const std::exception& exp) {
        std::cout << exp.what() << std::endl;
    }
}
