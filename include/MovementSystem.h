#pragma once
#include "System.h"
#include "Registry.h"
#include "Components.h"

namespace ces {
    class MovementSystem : public System {
    private:
        Registry& registry;

    public:
        MovementSystem(Registry& reg) : registry(reg) {}

        void runIt(Entity entity) override {

            if (!registry.has<Position>(entity) || !registry.has<Velocity>(entity)) {
                return;
            }

            auto& pos = registry.get<Position>(entity);
            auto& vel = registry.get<Velocity>(entity);

            pos.x += vel.dx;
            pos.y += vel.dy;
        }
    };
}