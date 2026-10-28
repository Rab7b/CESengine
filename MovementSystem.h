#pragma once
#include "System.h"
#include "types.h"

namespace ces {
    class MovementSystem : public ces::System {
    public:

        using ces::System::System;

        void runIt(ces::Entity& en) override {
            ces::Position& pos = reg.getPosByEntity(en);
            ces::Velocity& vel = reg.getVelByEntity(en);

            pos.x += vel.vx;
            pos.y += vel.vy;
        }
    };
}