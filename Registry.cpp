#include "include/Registry.h"

namespace ces {
    Registry::Registry() {
        this->nextId = 0;
        this->transforms.resize(maxEntities);
        this->velocities.resize(maxEntities);
        this->visuals.resize(maxEntities);
    }

    Registry::~Registry() {
        this->nextId = 0;
    }

    ces::Entity Registry::createEntity() {
        ces::Entity id;
        if (!freeIds.empty()) {
            id = freeIds.back();
            freeIds.pop_back();
        }
        else {
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
        if (!isAlive(entity)) {
            throw std::runtime_error("ECS Error: Invalid or destroyed entity reference!");
        }

        aliveStatus.set(entity, false);
        hasTransform.reset(entity);
        hasVelocity.reset(entity);
        hasVisual.reset(entity);

        transforms[entity] = {};
        velocities[entity] = {};
        visuals[entity].reset();

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

    ces::Position& Registry::getPosition(ces::Entity entity) {
        return transforms[entity];
    }

    ces::Velocity& Registry::getVelocity(ces::Entity entity) {
        return velocities[entity];
    }

    ces::Visual& Registry::getVisual(ces::Entity entity) {
        return *visuals[entity];
    }

    void Registry::updateMovement() {
        for (ces::Entity i = 0; i < nextId; ++i) {
            if (aliveStatus.test(i) && hasTransform.test(i) && hasVelocity.test(i)) {
                transforms[i].x += velocities[i].vx;
                transforms[i].y += velocities[i].vy;
            }
        }
    }

    ces::Entity Registry::getEntitiesCount() const {
        return this->nextId;
    }
}