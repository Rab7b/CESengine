#pragma once
#include <vector>
#include "types.h"

namespace ces {

    template <typename T>
    class ComponentPool {
    private:
        std::vector<T> data;
    public:
        ComponentPool() : data(maxEntities) {}

        inline T& get(const Entity& entity) {
            return data[entity.id];
        }

        inline void add(const Entity& entity, const T& component) {
            data[entity.id] = component;
        }

        inline void destroy(const Entity& entity) {
            data[entity.id] = T{};
        }
    };

}
