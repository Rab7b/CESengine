#include "Registry.h"

namespace ces {

    Entity Registry::createEntity() {
        Entity newId;
        if (!freeIds.empty()) {
            newId = freeIds.back();
            freeIds.pop_back();
        }
        else {
            if (nextId >= maxEntities) {
                throw std::runtime_error("Maximum entity limit reached!");
            }
            newId = nextId++;
        }
        aliveStatus.set(newId, true);
        return newId;
    }

    void Registry::destroyEntity(Entity entity) {
        if (!isAlive(entity)) return;

        for (auto& [typeIndex, pool] : pools) {
            pool->remove(entity);
        }

        aliveStatus.set(entity, false);
        freeIds.push_back(entity);
    }

    bool Registry::isAlive(Entity entity) const {
        if (entity >= maxEntities) return false;
        return aliveStatus.test(entity);
    }

    Entity Registry::getEntitiesCount() const {
        return nextId - static_cast<Entity>(freeIds.size());
    }
}