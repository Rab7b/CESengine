#pragma once
#include <vector>
#include "../Types/types.h"

namespace ces {

    template <typename T>
    class ComponentPool {
    private:
        std::vector<T> data;
    public:
        ComponentPool() : data(maxEntities) {}

        inline T& get(const Entity& entity) {
            return this->data[entity.id];
        }

        inline void add(const Entity& entity, const T& component) {
            this->data[entity.id] = component;
        }

        inline void destroy(const Entity& entity) {
            this->data[entity.id] = T{};
        }
    };

}
