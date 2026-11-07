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

        void runIt() override {
            ces::Entity count = reg.getEntitiesCount();
            for (ces::Entity i = 0; i < count; ++i) {
                if (!reg.isAlive(i)) continue;
                if (reg.has<Position>(i) && reg.has<Velocity>(i)) {
                    auto& pos = reg.get<Position>(i);
                    auto& vel = reg.get<Velocity>(i);
                    pos.x += vel.vx;
                    pos.y += vel.vy;
                }
            }
        }

    };
}