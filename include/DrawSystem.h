#pragma once
#include "System.h"
#include "types.h"
#include <SFML/Graphics.hpp>
#include <variant>
#include <type_traits>

namespace ces {
    class DrawSystem : public ces::System {
    private:
        ces::RenderWindow& window;

    public:
        DrawSystem(ces::Registry& r, ces::RenderWindow& win) : ces::System(r), window(win) {}

        void runIt(ces::Entity& en) override {

            ces::Visual& vis = reg.getVisual(en);
            ces::Position& pos= reg.getPosition(en);
            
            std::visit([&](auto& shape) {
                using T = std::decay_t<decltype(shape)>;
                if constexpr (!std::is_same_v<T, std::monostate>) {
                    shape.setPosition(ces::Vector2f(pos.x, pos.y));
                    window.draw(shape); 
                }
            }, vis.it);
        }
    };
}
