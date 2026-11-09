#pragma once
#include "System.h"
#include "Registry.h"
#include "types.h"
#include <SFML/Graphics.hpp>
#include <variant>
#include <vector>

namespace ces {
    class DrawSystem : public System {
    private:
        sf::RenderWindow& window;
        std::vector<sf::Vertex> vertexBuffer;
        mutable bool drawnThisFrame = false;

    public:

        DrawSystem(Registry& reg, sf::RenderWindow& win)
            : System(reg), window(win) {}

        void runIt(ces::Entity& entity) override {
            if (entity == 0) {
                drawnThisFrame = false;
            }

            if (drawnThisFrame) return;

            std::size_t count = reg.getEntitiesCount();

            if (vertexBuffer.size() < count) {
                vertexBuffer.resize(count);
            }

            std::size_t index = 0;
            for (Entity i = 0; i < count; ++i) {
                if (!reg.isAlive(i)) continue;

                if (reg.has<Position>(i) && reg.has<Visual>(i)) {
                    const auto& pos = reg.get<Position>(i);
                    const auto& vis = reg.get<Visual>(i);

                    sf::Color color = sf::Color::White;

                    if (auto* circle = std::get_if<sf::CircleShape>(&vis.it)) {
                        color = circle->getFillColor();
                    } else if (auto* rect = std::get_if<sf::RectangleShape>(&vis.it)) {
                        color = rect->getFillColor();
                    }

                    vertexBuffer[index].position = sf::Vector2f(pos.x, pos.y);
                    vertexBuffer[index].color = color;
                    index++;
                }
            }

            if (index > 0) {
                window.draw(vertexBuffer.data(), index, sf::PrimitiveType::Points);
            }

            drawnThisFrame = true;
        }
    };
}
