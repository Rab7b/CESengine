#pragma once
#include <tuple>
#include <vector>
#include "Managers/ComponentPool.hpp"

namespace ces {
    template <typename... Components>
    class Registry {
    private:
        std::tuple<ComponentPool<Components>...> componentPools;
        std::vector<ces::Entity> entityPool; 

    public:
        Registry() {
            entityPool.reserve(maxEntities);
            for (int i = maxEntities - 1; i >= 0; --i) {
                entityPool.push_back(ces::Entity(static_cast<uint32_t>(i)));
            }
        }

        inline ces::Entity createEntity() {
            ces::Entity e = entityPool.back();
            entityPool.pop_back();
            return e;
        }

        template <typename T>
        inline ComponentPool<T>& getPool() {
            return std::get<ComponentPool<T>>(componentPools);
        }

        inline void destroyEntity(const ces::Entity& entity) {
            ((this->getPool<Components>().destroy(entity)), ...);
            entityPool.push_back(entity);
        }

        template <typename... C>
        inline void addComponent(const ces::Entity& entity, const C&... components) {
            (this->getPool<C>().add(entity, components), ...);
        }

        template <typename... C>
        inline void removeComponent(const ces::Entity& entity, const C&... components) {
            (this->getPool<C>().destroy(entity), ...);
        }
    };
}
