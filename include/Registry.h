#pragma once

#include <vector>
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
        std::vector<ces::Visual> visuals;

        std::bitset<maxEntities> hasTransform;
        std::bitset<maxEntities> hasVelocity;
        std::bitset<maxEntities> hasVisual;

    public:
        Registry();
        ~Registry() = default;

        ces::Entity createEntity();
        void destroyEntity(ces::Entity entity);
        bool isAlive(ces::Entity entity) const;

        void setTransform(ces::Entity entity, const ces::Position& t);
        void setVelocity(ces::Entity entity, const ces::Velocity& v);
        void setVisual(ces::Entity entity, const ces::Visual& vis);

        ces::Position& getPosition(ces::Entity entity);
        ces::Velocity& getVelocity(ces::Entity entity);
        ces::Visual& getVisual(ces::Entity entity);

        void updateMovement();
        ces::Entity getEntitiesCount() const;
    };
}
