#pragma once
#include "System.h"
#include "Registry.h"
#include "types.h"

namespace ces {
    class MovementSystem : public System {
    private:
        Registry& registry;

    public:
        MovementSystem(Registry& reg) : registry(reg) {}

        void runIt(Entity& entity) override {

            if (!registry.has<ces::Position>(entity) || !registry.has<ces::Velocity>(entity)) {
                return;
            }

            auto& pos = registry.get<ces::Position>(entity);
            auto& vel = registry.get<ces::Velocity>(entity);

            pos.x += vel.vx;
            pos.y += vel.vy;
        }
    };
}
