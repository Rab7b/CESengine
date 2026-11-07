#pragma once
#include "System.h"
#include "types.h"

namespace ces {
    class MovementSystem : public System {
    public:
        MovementSystem(Registry& reg) : System(reg) {}

        void runIt(Entity& en) override {
            if (!reg.has<Position>(en) || !reg.has<Velocity>(en)) {
                return;
            }

            auto& pos = reg.get<Position>(en);
            auto& vel = reg.get<Velocity>(en);

            pos.x += vel.vx;
            pos.y += vel.vy;
        }
    };
}
