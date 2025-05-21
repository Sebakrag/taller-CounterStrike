#include "RenderSystem.h"

#include <iostream>

#include "client/model/ECS/components/InputComponent.h"

void RenderSystem::update(Renderer& ren, ComponentManager& cm) {
    // Por ahora solo muestro los inputs que se reciben. TODO: implementar el render correctamente.
    InputComponent* input = cm.getComponent<InputComponent>(
            1);  // Harcodee el id del local_player solo para probar.
    if (input == nullptr)
        return;

    std::cout << "[Shooting]: " << static_cast<int>(input->shooting) << "\n"
              << "[switch-weapon]: " << static_cast<int>(input->switch_weapon) << "\n"
              << "[vec2D]: " << input->move_dir << "\n"
              << std::endl;
    input->reset();
    ren.SetDrawColor(0, 0, 0, 0);
    ren.DrawLine(200, 200, 350, 400);
    ren.SetDrawColor(255, 255, 255, 255);
}
