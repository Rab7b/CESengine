#pragma once
#include "System.h"
#include "types.h"
#include <variant>

namespace ces {
    class DrawSystem : public System {
    private:
        sf::RenderWindow& window;

    public:
        DrawSystem(Registry& reg, sf::RenderWindow& win) : System(reg), window(win) {}

        void runIt(Entity& en) override {
            if (!reg.has<Position>(en) || !reg.has<Visual>(en)) {
                return;
            }

            auto& pos = reg.get<Position>(en);
            auto& visual = reg.get<Visual>(en);

            std::visit([&](auto& shape) {
                using T = std::decay_t<decltype(shape)>;
                if constexpr (!std::is_same_v<T, std::monostate>) {
                    shape.setPosition({ pos.x, pos.y });
                    window.draw(shape);
                }
                }, visual.it);
        }
    };
}
