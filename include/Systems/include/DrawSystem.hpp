#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Engine/Engine.h"
#include "../Systems/System.h"
#include "../Types/types.h"
#include "../Managers/Registry.hpp"

namespace ces {
    class DrawSystem : public System {
    private:
        sf::VertexBuffer vertices{ sf::PrimitiveType::Points, sf::VertexBuffer::Usage::Stream };
        std::vector<sf::Vertex> staging{};
        sf::RenderWindow& window;

    public:
        explicit DrawSystem(ces::Engine& e) : System(e), window(e.getWindow()) {
            this->staging.resize(maxEntities);
            if (!this->vertices.create(maxEntities)) {}
        }

        void update() override {
            auto& transformPool = this->engine.getRegistry().getPool<Transform>();

            for (int i = 0; i < maxEntities; ++i) {
                Entity entity(i);
                Transform& t = transformPool.get(entity);

                staging[i] = sf::Vertex{ {t.x, t.y}, t.color };
            }


            this->vertices.update(this->staging.data());
            this->window.draw(this->vertices);
        }
    };
}