#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include "Types/types.h"

namespace ces {

    template <typename T, size_t PageSize = 256>
    class ComponentPool {
    private:

        std::vector<std::unique_ptr<T[]>> pages;

        inline void ensure_page(size_t entity_id) {
            size_t page_idx = entity_id / PageSize;
            if (page_idx >= pages.size()) {
                pages.resize(page_idx + 1);
            }
            if (!pages[page_idx]) {
                pages[page_idx] = std::make_unique<T[]>(PageSize);
            }
        }

    public:
        ComponentPool() = default;

        inline T& get(const Entity& entity) {
            ensure_page(entity.id);
            return pages[entity.id / PageSize][entity.id % PageSize];
        }

        inline void add(const Entity& entity, const T& component) {
            ensure_page(entity.id);
            pages[entity.id / PageSize][entity.id % PageSize] = component;
        }

        inline void destroy(const Entity& entity) {
            size_t page_idx = entity.id / PageSize;
            if (page_idx < pages.size() && pages[page_idx]) {
                pages[page_idx][entity.id % PageSize] = T{};
            }
        }
    };

} 
