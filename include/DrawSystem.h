#pragma once
#include "System.h"
#include "include/types.h"
#include <SFML/Graphics.hpp>

namespace ces {
    class DrawSystem : public ces::System {
    private:
        ces::RenderWindow& window;

    public:
        DrawSystem(ces::Registry& r, ces::RenderWindow& win) : ces::System(r), window(win) {}

        void runIt(ces::Entity& en) override {
            ces::Visual& vis = reg.getVisual(en);

            if (vis.it) {
                window.draw(*vis.it);
            }
        }
    };
}