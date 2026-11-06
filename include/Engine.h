#pragma once
#include "DrawSystem.h"
#include "System.h"
#include "MovementSystem.h"
#include "Registry.h"
#include <vector>
#include <memory>
#include <optional>
#include <string>

extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;     // NVIDIA
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;       // AMD / ATI
}

namespace ces {
    class Engine {
    private:
        ces::Registry registry;
        sf::RenderWindow window;
        std::vector<std::unique_ptr<ces::System>> systems;

        inline void update() {
            window.clear();

            for (ces::Entity i = 0; i < this->registry.getEntitiesCount() * 2; ++i) {
                if (!this->registry.isAlive(i)) continue;
                for (const auto& system : this->systems) {
                    system->runIt(i);
                }
            }

            window.display();
        }

        inline Engine(unsigned int width, unsigned int height, const std::string& title, bool vSync = true, unsigned int frameRateLimit = 60)
            : window(sf::VideoMode({ width, height }), title) {
            window.setVerticalSyncEnabled(vSync);
            if (!vSync && frameRateLimit > 0) {
                window.setFramerateLimit(frameRateLimit);
            }
        }

    public:
        inline Engine(const ces::Engine&) = delete;
        inline Engine& operator=(const ces::Engine&) = delete;
        inline Engine(ces::Engine&&) = delete;
        inline Engine& operator=(ces::Engine&&) = delete;

        inline static ces::Engine& getInstance(unsigned int width = 1280, unsigned int height = 720, const std::string& title = "CES Engine", bool vSync = true, unsigned int frameRateLimit = 60) {
            static ces::Engine instance(width, height, title, vSync, frameRateLimit);
            return instance;
        }

        template<typename T, typename... Args>
        inline void addSystem(Args&&... args) {
            this->systems.push_back(std::make_unique<T>(this->registry, std::forward<Args>(args)...));
        }

        inline void run() {
            while (const std::optional event = this->getWindow().pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    this->getWindow().close();
                }
            }

            this->update();
        }

        inline sf::RenderWindow& getWindow() {
            return this->window;
        }

        inline Registry& getRegistry() {
            return this->registry;
        }

        inline bool isWindowOpen() {
            return this->getWindow().isOpen();
        }

        template<typename S>
        inline S* getSystem() {
            for (auto& system : this->systems) {
                if (auto derived = dynamic_cast<S*>(system.get())) {
                    return derived;
                }
            }
            return nullptr;
        }
    };
}