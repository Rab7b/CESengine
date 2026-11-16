#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Systems/System.h" 
#include "Managers/Registry.hpp"

namespace ces {

    class System;

    class Engine {
    private:

        sf::Clock m_clock;
        sf::Time m_accumulatedTime = sf::Time::Zero;
        int m_frameCount = 0;
        float m_currentFps = 0.0f;

        ces::Registry<ces::Transform, ces::Velocity> r;

        std::vector<std::unique_ptr<System>> systems;

        ces::RenderWindow window{};

        Engine() : window(ces::VideoMode({ 800, 600 }), "C++ ECS Engine") {}

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

    public:
        static Engine& getInstance(ces::Vector2f size, std::string name, bool vsync, float fps) {
            static Engine instance;
            instance.getWindow().setFramerateLimit(fps);
            instance.getWindow().setVerticalSyncEnabled(vsync);
            instance.getWindow().setSize({ static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y) });
            return instance;
        }

        template <typename T, typename... Args>
        inline void addSystem(Args&&... args) {
            systems.push_back(std::make_unique<T>(*this, std::forward<Args>(args)...));
        }

        inline ces::RenderWindow& getWindow() {
            return this->window;
        }

        inline auto& getRegistry() {
            return this->r;
        }

		int getFPS() const {
			return static_cast<int>(this->m_currentFps);
		}

        inline void run() {

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
    };
}
