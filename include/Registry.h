#pragma once
#include "System.h"
#include "types.h"

namespace ces {
    class MovementSystem : public System {
    public:
        MovementSystem(Registry& reg) : System(reg) {}

        void runIt() override {
            auto& pos_pool = reg.getPool<Position>();
            auto& vel_pool = reg.getPool<Velocity>();

            if (pos_pool.size() < vel_pool.size()) {

                for (size_t i = 0; i < pos_pool.size(); ++i) {
                    Entity entity = pos_pool.get_entity_at(i);

                    if (vel_pool.has(entity)) {
                        auto& pos = pos_pool.get_component_at(i); 
                        auto& vel = vel_pool.get(entity);
                        pos.x += vel.vx;
                        pos.y += vel.vy;
                    }
                }
            } else {
                for (size_t i = 0; i < vel_pool.size(); ++i) {
                    Entity entity = vel_pool.get_entity_at(i);
                    
                    if (pos_pool.has(entity)) {
                        auto& vel = vel_pool.get_component_at(i);
                        auto& pos = pos_pool.get(entity);
                        pos.x += vel.vx;
                        pos.y += vel.vy;
                    }
                }
            }
        }
    };
}
