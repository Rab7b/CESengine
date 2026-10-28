#pragma once
#include "System.h"
#include "types.h"
#include <SFML/Graphics.hpp>

namespace ces {
    class DrawSystem : public ces::System {
    private:
        ces::RenderWindow& window;

    public:
        DrawSystem(ces::Registry& r, ces::RenderWindow& win) : ces::System(r), window(win) {}

        void runIt(ces::Entity& en) override {
            ces::Position& pos = reg.getPosByEntity(en);
            ces::Visual& vis = reg.getVisByEntity(en);

            if (vis.it) {
                vis.it->setPosition({ pos.x, pos.y });
                window.draw(*vis.it);
            }
        }
    };
}