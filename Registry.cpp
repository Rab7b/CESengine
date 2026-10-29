#include "Registry.h"

namespace ces {

    Registry::Registry() {
        this->nextId = 0;
        this->transforms.resize(maxEntities);
        this->velocities.resize(maxEntities);
        this->visuals.resize(maxEntities);
    }

    ces::Entity Registry::createEntity() {
        ces::Entity id;
        if (!freeIds.empty()) {
            id = freeIds.back();
            freeIds.pop_back();
        } else {
            if (nextId >= maxEntities) {
                throw std::runtime_error("ECS Error: Exceeded maximum entity capacity!");
            }
            id = nextId++;
        }

        aliveStatus.set(id, true);
        hasTransform.reset(id);
        hasVelocity.reset(id);
        hasVisual.reset(id);
        return id;
    }

    void Registry::destroyEntity(ces::Entity entity) {
        if (!isAlive(entity)) return;

        aliveStatus.set(entity, false);
        hasTransform.reset(entity);
        hasVelocity.reset(entity);
        hasVisual.reset(entity);

        transforms[entity] = {};
        velocities[entity] = {};
        visuals[entity] = {};

        freeIds.push_back(entity);
    }

    bool Registry::isAlive(ces::Entity entity) const {
        return entity < maxEntities && aliveStatus.test(entity);
    }

    void Registry::setTransform(ces::Entity entity, const ces::Position& t) {
        if (!isAlive(entity)) return;
        transforms[entity] = t;
        hasTransform.set(entity, true);
    }

    void Registry::setVelocity(ces::Entity entity, const ces::Velocity& v) {
        if (!isAlive(entity)) return;
        velocities[entity] = v;
        hasVelocity.set(entity, true);
    }

    void Registry::setVisual(ces::Entity entity, const ces::Visual& vis) {
        if (!isAlive(entity)) return;
        visuals[entity] = vis;
        hasVisual.set(entity, true);
    }

    ces::Position& Registry::getPosition(ces::Entity entity) {
        if (!hasTransform.test(entity)) throw std::runtime_error("No Transform Component!");
        return transforms[entity];
    }

    ces::Velocity& Registry::getVelocity(ces::Entity entity) {
        if (!hasVelocity.test(entity)) throw std::runtime_error("No Velocity Component!");
        return velocities[entity];
    }

    ces::Visual& Registry::getVisual(ces::Entity entity) {
        if (!hasVisual.test(entity)) throw std::runtime_error("No Visual Component!");
        return visuals[entity];
    }

    void Registry::updateMovement() {
        auto activeMovementMask = aliveStatus & hasTransform & hasVelocity;

        ces::Position* posPtr = transforms.data();
        ces::Velocity* velPtr = velocities.data();

        for (ces::Entity i = 0; i < nextId; ++i) {
            if (activeMovementMask.test(i)) {
                posPtr[i].x += velPtr[i].vx;
                posPtr[i].y += velPtr[i].vy;
            }
        }
    }

    ces::Entity Registry::getEntitiesCount() const {
        return this->nextId;
    }
}
