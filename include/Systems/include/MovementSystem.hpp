#pragma once
#include <algorithm>
#include <cstddef>
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
            auto& tData = registry.getPool<Transform>().getData();
            const auto& vData = registry.getPool<Velocity>().getData();

            const int count = static_cast<int>(std::min(tData.size(), vData.size()));

#pragma omp parallel for schedule(static)
            for (int i = 0; i < count; ++i) {
                tData[i].x += vData[i].vx;
                tData[i].y += vData[i].vy;
            }
        }
    };
}