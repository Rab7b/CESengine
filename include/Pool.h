#pragma once
#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>
#include "types.h"

namespace ces {

    constexpr size_t TOMBSTONE = static_cast<size_t>(-1);

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
        std::vector<Entity> dense;       
        std::vector<size_t> sparse;      

    public:
        template<typename... Args>
        T& emplace(Entity entity, Args&&... args) {
            if (entity >= sparse.size()) {
                sparse.resize(entity + 1, TOMBSTONE);
            }

            size_t idx = sparse[entity];

            if (idx != TOMBSTONE && idx (args)... };
                return data[idx];
            }

            sparse[entity] = dense.size();
            dense.push_back(entity);
            data.emplace_back(std::forward<Args>(args)...);

            return data.back();
        }

        T& get(Entity entity) {
            return data[sparse[entity]];
        }

        const T& get(Entity entity) const {
            return data[sparse[entity]];
        }

        bool has(Entity entity) const override {
            if (entity >= sparse.size()) return false;
            size_t idx = sparse[entity];
            return idx != TOMBSTONE && idx < dense.size() && dense[idx] == entity;
        }

        void remove(Entity entity) override {
            if (!has(entity)) return;

            size_t idx_to_remove = sparse[entity];
            size_t last_idx = dense.size() - 1;
            Entity last_entity = dense[last_idx];

            if (idx_to_remove != last_idx) {
                data[idx_to_remove] = std::move(data[last_idx]);
                dense[idx_to_remove] = last_entity;
                sparse[last_entity] = idx_to_remove; 
            }

            sparse[entity] = TOMBSTONE;

            data.pop_back();
            dense.pop_back();
        }

        size_t size() const { return dense.size(); }
        auto begin() { return data.begin(); }
        auto end() { return data.end(); }
    };
}
