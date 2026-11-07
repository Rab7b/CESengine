#pragma once
#include <vector>
#include <memory>
#include <bitset>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <typeindex>
#include "types.h"
#include "Pool.h"

namespace ces {

    class Registry {
    private:
        std::bitset<maxEntities> aliveStatus;
        ces::Entity nextId = 0;

        std::vector<ces::Entity> freeIds;
        std::vector<ces::Entity> activeEntities;

        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> pools;

        template<typename T>
        ComponentPool<T>& getOrCreatePool() {
            std::type_index key(typeid(T));
            auto it = pools.find(key);
            if (it == pools.end()) {
                auto newPool = std::make_unique<ComponentPool<T>>();
                auto& poolRef = *newPool;
                pools[key] = std::move(newPool);
                return poolRef;
            }
            return *static_cast<ComponentPool<T>*>(it->second.get());
        }

    public:
        Registry() = default;
        ~Registry() = default;

        ces::Entity createEntity();
        void destroyEntity(ces::Entity entity);
        bool isAlive(ces::Entity entity) const;

        const std::vector<ces::Entity>& getActiveEntities() const {
            return activeEntities;
        }

        template<typename T, typename... Args>
        T& assign(ces::Entity entity, Args&&... args) {
            if (!this->isAlive(entity)) {
                throw std::runtime_error("Attempted to assign component to a dead entity!");
            }
            return getOrCreatePool<T>().emplace(entity, std::forward<Args>(args)...);
        }

        template<typename T>
        T& get(ces::Entity entity) {
            return getOrCreatePool<T>().get(entity);
        }

        template<typename T>
        bool has(ces::Entity entity) const {
            std::type_index key(typeid(T));
            auto it = pools.find(key);
            if (it == pools.end()) return false;
            return static_cast<const ComponentPool<T>*>(it->second.get())->has(entity);
        }

        ces::Entity getEntitiesCount() const;
    };
}
