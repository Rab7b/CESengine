#include "Engine/Engine.h"

namespace ces {
	void Engine::run() {

        while (const std::optional event = this->window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->window.close();
            }
		}

		this->window.clear();
		this->window.display();
	}
}

