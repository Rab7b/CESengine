#pragma once
#include <SFML/Graphics.hpp>
#include "Engine/Engine.h"
#include "Systems/System.h"
#include "types.h"
#include "Registry.hpp"

namespace ces {
    class DrawSystem : public System {
    private:
        sf::VertexArray vertices{};
        sf::RenderWindow& window;

    public:
        explicit DrawSystem(ces::Engine& e) : System(e), window(e.getWindow()) {}

        void update() override {
            this->vertices.clear();

            auto& transformPool = engine.getRegistry().getPool<Transform>();

            for (int i = 0; i < maxEntities; i++) {
                Entity entity(i);
                Transform& t = transformPool.get(entity);

                sf::Vertex v1{ {t.x, t.y}, sf::Color::White };
                this->vertices.append(v1);
            }

            this->window.draw(this->vertices);
        }
    };
}