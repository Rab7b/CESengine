#pragma once
#include "System.h"
#include "types.h"

namespace ces {
    class MovementSystem : public ces::System {
    public:

        using ces::System::System;

        void runIt(ces::Entity& en) override {
            ces::Position& pos = reg.get<ces::Position>(en);
            ces::Velocity& vel = reg.get<ces::Velocity>(en);

            pos.x += vel.vx;
            pos.y += vel.vy;
        }
    };
}