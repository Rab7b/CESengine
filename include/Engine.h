#pragma once

#include "DrawSystem.h"
#include "System.h"
#include "MovementSystem.h"
#include "Registry.h"

namespace ces {
    class Engine {
    private:

        ces::Registry registry;
        ces::RenderWindow window;

        std::vector<std::unique_ptr<ces::System>> systems;


        inline void update() {

            window.clear();

            for (ces::Entity i = 0; i < this->registry.getEntitiesCount(); ++i) {
                for (const auto& system : this->systems) {
                    system->runIt(i);
                }
            }

            window.display();
        }

        inline Engine() : window(ces::VideoMode({ 800, 600 }), "CES Engine") {}

    public:

        Engine(const ces::Engine&) = delete;
        Engine& operator=(const ces::Engine&) = delete;
        Engine(ces::Engine&&) = delete;
        Engine& operator=(ces::Engine&&) = delete;

        inline static ces::Engine& getInstance() {
            static ces::Engine instance;
            return instance;
        }

        template<typename T, typename... Args>
        inline void addSystem(Args&&... args) {
            this->systems.push_back(std::make_unique<T>(this->registry, std::forward<Args>(args)...));
        }

        inline void run() {

            while (const std::optional event = this->getWindow().pollEvent()) {
                if (event->is<ces::Event::Closed>()) {
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