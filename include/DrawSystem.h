#pragma once
#include "System.h"
#include "Registry.h"
#include "Components.h"
#include <SFML/Graphics.hpp>

namespace ces {
    class DrawSystem : public System {
    private:
        Registry& registry;
        sf::RenderWindow& window;

    public:

        DrawSystem(Registry& reg, sf::RenderWindow& win) : registry(reg), window(win) {}

        void runIt(Entity entity) override {
            if (!registry.has<Position>(entity)) {
                return;
            }

            auto& pos = registry.get<Position>(entity);

            if (registry.has<ces::Visual>(entity)) {
                auto& spriteComp = registry.get<ces::Visual>(entity);
                spriteComp.it.setPosition({ pos.x, pos.y });
                window.draw(spriteComp.it);
            }
        }
    };
}