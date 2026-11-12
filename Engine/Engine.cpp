#include "Engine.h"

namespace ces {
    void Engine::run() {
        ces::Clock deltaClock;

        while (this->window.isOpen()) {

            float elapsed = deltaClock.restart().asSeconds();
            if (elapsed > 0.0f) {
                this->currentFps = 1.0f / elapsed;
            }

            this->window.setTitle("CES Engine | FPS: " + std::to_string(static_cast<int>(this->currentFps)));

            while (const std::optional event = this->window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    this->window.close();
                }
            }

            this->window.clear();

            for (auto& system : systems) {
                system->update();
            }

            this->window.display();
        }
    }
}