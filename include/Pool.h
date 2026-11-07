#pragma once
#include <vector>
#include "types.h"

namespace ces {

    class IComponentPool {
    public:
        virtual ~IComponentPool() = default;
        virtual void remove(Entity entity) = 0;
        virtual bool has(Entity entity) const = 0;
    };

    template<typename T>
    class ComponentPool : public IComponentPool {
    private:
        std::vector<T> data;
        std::vector<uint8_t> occupied;

    public:
        template<typename... Args>
        T& emplace(Entity entity, Args&&... args) {
            if (entity >= data.size()) {
                data.resize(entity + 1);
                occupied.resize(entity + 1, false);
            }
            data[entity] = T{ std::forward<Args>(args)... };
            occupied[entity] = true;
            return data[entity];
        }

        T& get(Entity entity) {
            return data[entity];
        }

        const T& get(Entity entity) const {
            return data[entity];
        }

        bool has(Entity entity) const override {
            return entity < occupied.size() && occupied[entity];
        }

        void remove(Entity entity) override {
            if (entity < occupied.size()) {
                occupied[entity] = false;
            }
        }
    };
}
