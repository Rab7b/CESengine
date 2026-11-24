#pragma once
#include <cstdint>
#include <tuple>
#include <vector>
#include "Managers/ComponentPool.hpp"

namespace ces {
    template <typename... Components>
    class Registry {
    private:
        std::tuple<ComponentPool<Components>...> componentPools;
        std::vector<ces::Entity> freeEntities;
        uint32_t nextEntityId = 0;

    public:
        Registry() = default;

        inline ces::Entity createEntity() {
            if (!freeEntities.empty()) {
                ces::Entity e = freeEntities.back();
                freeEntities.pop_back();
                return e;
            }
            return ces::Entity(nextEntityId++);
        }

        template <typename T>
        inline ComponentPool<T>& getPool() {
            return std::get<ComponentPool<T>>(componentPools);
        }

        inline void destroyEntity(const ces::Entity& entity) {
            ((this->getPool<Components>().destroy(entity)), ...);
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