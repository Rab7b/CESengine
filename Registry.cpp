#include "include/Registry.h"

namespace ces {

    Registry::Registry() {
        this->nextId = 0;
        this->transforms.resize(maxEntities);
        this->velocities.resize(maxEntities);
        this->visuals.resize(maxEntities);
    }

    ces::Entity Registry::createEntity() {
        ces::Entity id;
        if (!this->freeIds.empty()) {
            id = this->freeIds.back();
            this->freeIds.pop_back();
        }
        else {
            if (this->nextId >= maxEntities) {
                throw std::runtime_error("ECS Error: Exceeded maximum entity capacity!");
            }
            id = this->nextId++;
        }

        this->aliveStatus.set(id, true);
        this->hasTransform.reset(id);
        this->hasVelocity.reset(id);
        this->hasVisual.reset(id);
        return id;
    }

    void Registry::destroyEntity(ces::Entity entity) {
        if (!this->isAlive(entity)) return;

        this->aliveStatus.set(entity, false);
        this->hasTransform.reset(entity);
        this->hasVelocity.reset(entity);
        this->hasVisual.reset(entity);

        this->transforms[entity] = {};
        this->velocities[entity] = {};
        this->visuals[entity].reset();

        this->freeIds.push_back(entity);
    }

    bool Registry::isAlive(ces::Entity entity) const {
        return entity < maxEntities && this->aliveStatus.test(entity);
    }

    void Registry::setTransform(ces::Entity entity, const ces::Position& t) {
        if (!this->isAlive(entity)) return;
        this->transforms[entity] = t;
        this->hasTransform.set(entity, true);
    }

    void Registry::setVelocity(ces::Entity entity, const ces::Velocity& v) {
        if (!this->isAlive(entity)) return;
        this->velocities[entity] = v;
        this->hasVelocity.set(entity, true);
    }

    ces::Position& Registry::getPosition(ces::Entity entity) {
        if (!this->hasTransform.test(entity)) throw std::runtime_error("No Transform Component!");
        return this->transforms[entity];
    }

    ces::Velocity& Registry::getVelocity(ces::Entity entity) {
        if (!this->hasVelocity.test(entity)) throw std::runtime_error("No Velocity Component!");
        return this->velocities[entity];
    }

    ces::Visual& Registry::getVisual(ces::Entity entity) {
        if (!this->hasVisual.test(entity)) throw std::runtime_error("No Visual Component!");
        return *this->visuals[entity];
    }

    ces::Entity Registry::getEntitiesCount() const {
        return this->nextId;
    }
}