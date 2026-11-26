#pragma once
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include "Managers/ComponentPool.hpp"

namespace ces {
    template <typename... Components>
    class Registry {
    private:
        std::vector<std::unique_ptr<IComponentPool>> componentPools;
        std::vector<ces::Entity> freeEntities;
        uint32_t nextEntityId = 0;

        static inline std::size_t nextTypeId = 0;

        template <typename T>
        static std::size_t getTypeId() {
            static const std::size_t id = nextTypeId++;
            return id;
        }

        template <typename T, size_t PAGE_SIZE = 4096>
        void ensurePoolExists() {
            const std::size_t id = getTypeId<T>();
            if (id >= componentPools.size()) {
                componentPools.resize(id + 1);
            }
            if (!componentPools[id]) {
                componentPools[id] = std::make_unique<ComponentPool<T, PAGE_SIZE>>();
            }
        }

    public:
        Registry() {
            (ensurePoolExists<Components>(), ...);
        }

        inline ces::Entity createEntity() {
            if (!freeEntities.empty()) {
                ces::Entity e = freeEntities.back();
                freeEntities.pop_back();
                return e;
            }
            return ces::Entity(nextEntityId++);
        }

        template <typename T, size_t PAGE_SIZE = 4096>
        inline void addPool(ComponentPool<T, PAGE_SIZE>&& pool) {
            const std::size_t id = getTypeId<T>();
            if (id >= componentPools.size()) {
                componentPools.resize(id + 1);
            }
            componentPools[id] = std::make_unique<ComponentPool<T, PAGE_SIZE>>(std::move(pool));
        }

        template <typename T, size_t PAGE_SIZE = 4096>
        inline ComponentPool<T, PAGE_SIZE>& getPool() {
            ensurePoolExists<T, PAGE_SIZE>();
            return *static_cast<ComponentPool<T, PAGE_SIZE>*>(componentPools[getTypeId<T>()].get());
        }

        inline void destroyEntity(const ces::Entity& entity) {
            for (auto& pool : componentPools) {
                if (pool) {
                    pool->destroy(entity);
                }
            }
            freeEntities.push_back(entity);
        }

        template <typename... C>
        inline void addComponent(const ces::Entity& entity, const C&... components) {
            (this->getPool<C>().add(entity, components), ...);
        }

        template <typename... C>
        inline void removeComponent(const ces::Entity& entity) {
            (this->getPool<C>().destroy(entity), ...);
        }
    };
}
