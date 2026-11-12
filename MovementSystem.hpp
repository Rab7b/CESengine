#pragma once
#include "Engine/Engine.h"
#include "Systems/System.h"
#include "types.h"
#include "Registry.hpp"

namespace ces {
    class MovementSystem : public System {
    public:
        using System::System;

        void update() override {
            auto& registry = engine.getRegistry();
            auto& transformPool = registry.getPool<Transform>();
            auto& velocityPool = registry.getPool<Velocity>();

            for (uint32_t i = 0; i < maxEntities; ++i) {
                Entity entity{ i };
                auto& t = transformPool.get(entity);
                auto& v = velocityPool.get(entity);

                t.x += v.vx;
                t.y += v.vy;
            }
        }
    };
}