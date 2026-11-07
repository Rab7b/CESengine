#pragma once
#include "System.h"
#include "types.h"

namespace ces {
    class MovementSystem : public System {
    public:
        MovementSystem(Registry& reg) : System(reg) {}

<<<<<<< HEAD
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
=======
        void runIt(Entity& en) override {
            if (!reg.has<Position>(en) || !reg.has<Velocity>(en)) {
                return;
            }

            auto& pos = reg.get<Position>(en);
            auto& vel = reg.get<Velocity>(en);

            pos.x += vel.vx;
            pos.y += vel.vy;
>>>>>>> 725f5e6122dde2a2520188ca188cba177710c5ea
        }

    };
}
