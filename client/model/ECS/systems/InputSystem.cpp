#include "InputSystem.h"

#include "client/model/ECS/components/InputComponent.h"
#include "client/model/ECS/components/PositionComponent.h"

// La idea seria que cuando recibimos los datos del server, actualizamos los InputComponent
// de cada player. Despues esos datos se cargan aca en cada componente en particular.
void InputSystem::update(float delta_time, ComponentManager& cm) {
    auto input_arr = cm.getComponentArray<InputComponent>();
    auto entt_w_input = input_arr.getEntities();
    auto transform_arr = cm.getComponentArray<TransformComponent>();

    for (auto& entt : entt_w_input) {
        auto* input = input_arr.get(entt);
        auto* t = transform_arr.get(entt);

        t->position += input->move_dir;
    }
}
