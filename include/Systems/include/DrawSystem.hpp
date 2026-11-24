#pragma once
#include <SFML/Graphics.hpp>
#include <cstddef>
#include <vector>
#include "Engine/Engine.h"
#include "Systems/System.h"
#include "Types/types.h"
#include "Managers/Registry.hpp"

namespace ces {
    class DrawSystem : public System {
    private:
        sf::VertexBuffer vertices{ sf::PrimitiveType::Points, sf::VertexBuffer::Usage::Stream };
        std::vector<sf::Vertex> staging{};
        sf::RenderWindow& window;
        std::size_t allocatedCount = 0;

    public:
        explicit DrawSystem(ces::Engine& e) : System(e), window(e.getWindow()) {}

        void update() override {
            const auto& tData = this->engine.getRegistry().getPool<Transform>().getData();
            const std::size_t count = tData.size();
            if (count == 0) {
                return;
            }

            if (count != this->allocatedCount) {
                this->staging.resize(count);
                if (!this->vertices.create(count)) {
                    return;
                }
                this->allocatedCount = count;
            }

#pragma omp parallel for schedule(static)
            for (int i = 0; i < static_cast<int>(count); ++i) {
                const Transform& t = tData[i];
                this->staging[i] = sf::Vertex{ {t.x, t.y}, t.color };
            }

            this->vertices.update(this->staging.data());
            this->window.draw(this->vertices);
        }
    };
}