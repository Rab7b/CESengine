#pragma once
#include "System.h"
#include "Components.h"
#include <variant>

namespace ces {
    class DrawSystem : public System {
    private:
        sf::RenderWindow& window;

    public:
        DrawSystem(Registry& reg, sf::RenderWindow& win) : System(reg), window(win) {}

        void runIt() override {

            std::size_t count = reg.getEntitiesCount();
            sf::VertexArray vertices(sf::PrimitiveType::Points);
            vertices.resize(count);

            std::size_t index = 0;
            for (Entity i = 0; i < count; ++i) {
                if (!reg.isAlive(i)) continue;
                if (reg.has<Position>(i) && reg.has<Visual>(i)) {
                    const auto& pos = reg.get<Position>(i);
                    const auto& vis = reg.get<Visual>(i);

                    sf::Color color = sf::Color::White;
                    std::visit([&](auto& shape) {
                        using T = std::decay_t<decltype(shape)>;
                        if constexpr (std::is_same_v<T, sf::CircleShape> || std::is_same_v<T, sf::RectangleShape>) {
                            color = shape.getFillColor();
                        }
                        }, vis.it);

                    vertices[index].position = sf::Vector2f(pos.x, pos.y);
                    vertices[index].color = color;
                    index++;
                }
            }

            vertices.resize(index);
            window.draw(vertices);
        }

    };
}