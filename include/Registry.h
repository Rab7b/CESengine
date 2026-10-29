#pragma once

#include <vector>
#include <memory>
#include <bitset>
#include <cstdint>
#include <stdexcept>
#include "types.h"

namespace ces {

    class Registry {
    private:
        std::bitset<maxEntities> aliveStatus;
        ces::Entity nextId = 0;
        std::vector<ces::Entity> freeIds;

        std::vector<ces::Position> transforms;
        std::vector<ces::Velocity> velocities;
        std::vector<std::unique_ptr<ces::Visual>> visuals;

        std::bitset<maxEntities> hasTransform;
        std::bitset<maxEntities> hasVelocity;
        std::bitset<maxEntities> hasVisual;

    public:
        Registry();
        ~Registry();

        ces::Entity createEntity();
        void destroyEntity(ces::Entity entity);
        bool isAlive(ces::Entity entity) const;

        void setTransform(ces::Entity entity, const ces::Position& t);
        void setVelocity(ces::Entity entity, const ces::Velocity& v);

        template<typename... Args>
        void assignVisual(ces::Entity entity, Args&&... args) {
            if (!isAlive(entity)) return;
            visuals[entity] = std::make_unique<ces::Visual>(std::forward<Args>(args)...);
            hasVisual.set(entity, true);
        }

        ces::Position& getPosition(ces::Entity entity);
        ces::Velocity& getVelocity(ces::Entity entity);
        ces::Visual& getVisual(ces::Entity entity);

        void updateMovement();
        ces::Entity getEntitiesCount() const;
    };
}