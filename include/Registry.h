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
        inline Registry();
        inline ~Registry() = default;

        inline ces::Entity createEntity();
        inline void destroyEntity(ces::Entity entity);
        inline bool isAlive(ces::Entity entity) const;

        inline void setTransform(ces::Entity entity, const ces::Position& t);
        inline void setVelocity(ces::Entity entity, const ces::Velocity& v);

        template<typename ShapeType, typename... Args>
        inline void assignVisual(ces::Entity entity, Args&&... args) {
            if (!isAlive(entity)) return;
            
            visuals[entity].emplace(Visual{ ShapeType(std::forward<Args>(args)...) });
    
            hasVisual.set(entity, true);
            if (!this->isAlive(entity)) return;

            this->visuals[entity].emplace(Visual{ ShapeType(std::forward<Args>(args)...) });
            
            this->visuals[entity].shape.emplace<ShapeType>(std::forward<Args>(args)...);
    
            this->hasVisual.set(entity, true);
        }



        inline ces::Position& getPosition(ces::Entity entity);
        inline ces::Velocity& getVelocity(ces::Entity entity);
        inline ces::Visual& getVisual(ces::Entity entity);

        inline ces::Entity getEntitiesCount() const;
    };
}
