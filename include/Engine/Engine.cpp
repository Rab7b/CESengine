#include "../Engine/Engine.h"
#include <iostream>

namespace ces {
    void Engine::run() {

        while (this->window.isOpen()) {

            sf::Time elapsed = m_clock.restart();
            this->m_accumulatedTime += elapsed;
            this->m_frameCount++;

            if (this->m_accumulatedTime.asSeconds() >= 1.0f) {
                this->m_currentFps = static_cast<float>(this->m_frameCount) / this->m_accumulatedTime.asSeconds();
                this->m_frameCount = 0;
                this->m_accumulatedTime = sf::Time::Zero;
            }

            while (const std::optional event = this->window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    this->window.close();
                }
            }

            this->window.clear();

            for (auto& system : this->systems) {
                system->update();
            }

            this->window.display();
        }
    }
}