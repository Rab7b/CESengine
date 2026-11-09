#pragma once
#include "System.h"
#include "Registry.h"
#include "types.h"
#include <SFML/Graphics.hpp>
#include <vector>

namespace ces {
    class DrawSystem : public System {
    private:
        Registry& registryRef;
        sf::RenderWindow& window;

        sf::VertexBuffer gpuBuffer;
        std::vector<sf::Vertex> vertexBuffer;

    public:
        DrawSystem(Registry& reg, sf::RenderWindow& win)
            : System(reg), registryRef(reg), window(win) {

            gpuBuffer.create(maxEntities);
            gpuBuffer.setPrimitiveType(sf::PrimitiveType::Points);
            gpuBuffer.setUsage(sf::VertexBuffer::Usage::Stream);
        }

        void runIt() override {

            auto& posPool = reg.getPool<Position>();
            auto& visPool = reg.getPool<Visual>();

            std::size_t count = reg.getEntitiesCount();
            if (vertexBuffer.size() < count) {
                vertexBuffer.resize(count);
            }

            std::size_t index = 0;

            for (Entity i : reg.getActiveEntities()) {
                if (posPool.has(i) && visPool.has(i)) {
                    const auto& pos = posPool.get(i);
                    const auto& vis = visPool.get(i);

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
                gpuBuffer.update(vertexBuffer.data(), index, 0);
                window.draw(gpuBuffer); 
            }
        }

        void runIt(Entity& en) override {}
    };
}
