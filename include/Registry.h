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

        template<typename ShapeType, typename... Args>
        void assignVisual(ces::Entity entity, Args&&... args) {
            if (!this->isAlive(entity)) return;

            this->visuals[entity].it.emplace<ShapeType>(std::forward<Args>(args)...);
            this->hasVisual.set(entity, true);
        }

        ces::Position& getPosition(ces::Entity entity);
        ces::Velocity& getVelocity(ces::Entity entity);
        ces::Visual& getVisual(ces::Entity entity);

        ces::Entity getEntitiesCount() const;
    };
}
