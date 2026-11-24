#pragma once
#include <cstddef>
#include <cstdint>
#include "Engine/Engine.h"
#include "Systems/System.h"
#include "Types/types.h"
#include "Managers/Registry.hpp"

namespace ces {
    class MovementSystem : public System {
    public:
        using System::System;

        void update() override {
            auto& registry = this->engine.getRegistry();
            auto& tPool = registry.getPool<Transform>();
            const auto& vPool = registry.getPool<Velocity>();

            const auto& vData = vPool.getData();
            const auto& vEntities = vPool.getEntities();
            const int count = static_cast<int>(vData.size());
            
#pragma omp parallel for schedule(static)
            for (int i = 0; i < count; ++i) {
                Entity entity{};
                entity.id = vEntities[i];
                if (tPool.has(entity)) {
                    Transform& t = tPool.get(entity);
                    t.x += vData[i].vx;
                    t.y += vData[i].vy;
                }
            }

        }
    };
}
