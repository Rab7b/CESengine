#include <SFML/Graphics.hpp>
#include "Engine.h"
#include "MovementSystem.h"
#include "DrawSystem.h"
#include "KeyboardSystem.h"

int main() {
    ces::Engine& engine = ces::Engine::getInstance();

    engine.addSystem<ces::KeyboardSystem>();
    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>(engine.getWindow());

    ces::Registry& reg = engine.getRegistry();
    ces::Entity e = reg.createEntity();

    reg.getPosByEntity(e) = { 400.0f, 300.0f };
    reg.getVelByEntity(e) = { 0.f, 0.f };

    auto& vis = reg.getVisByEntity(e);
    vis.it = std::make_unique<ces::CircleShape>(20.f);
    vis.it->setFillColor(ces::Color::White);

    while (engine.isWindowOpen()) {

        reg.getVelByEntity(e) = { 0.0f, 0.0f };

        if (auto keyboardSys = engine.getSystem<ces::KeyboardSystem>()) {
            ces::Keyboard::Key pressed = keyboardSys->getPressedKey();

            if (pressed == ces::Keyboard::Key::Escape) {
                engine.getWindow().close();
            }

            if (pressed == ces::Keyboard::Key::W || pressed == ces::Keyboard::Key::Up) {
                reg.getVelByEntity(e).vy = -0.1f;
            }
            if (pressed == ces::Keyboard::Key::S || pressed == ces::Keyboard::Key::Down) {
                reg.getVelByEntity(e).vy = 0.1f;
            }
            if (pressed == ces::Keyboard::Key::A || pressed == ces::Keyboard::Key::Left) {
                reg.getVelByEntity(e).vx = -0.1f;
            }
            if (pressed == ces::Keyboard::Key::D || pressed == ces::Keyboard::Key::Right) {
                reg.getVelByEntity(e).vx = 0.1f;
            }
        }


        engine.run();
    }

    return 0;
}