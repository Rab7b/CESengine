#pragma once
#include <array>
#include <cstddef>
#include <limits>
#include <memory>
#include <utility>
#include <vector>
#include "../Types/types.h"

namespace ces {

    template <typename T, size_t PAGE_SIZE = 4096>
    class ComponentPool {
    private:
        static constexpr size_t INVALID = maxEntities;
        using Page = std::array<size_t, PAGE_SIZE>;

        std::vector<std::unique_ptr<Page>> sparsePages;
        std::vector<T> data;
        std::vector<size_t> denseToEntity;

        inline size_t getDenseIndex(size_t entityId) const {
            size_t pageIdx = entityId / PAGE_SIZE;
            if (pageIdx >= sparsePages.size() || !sparsePages[pageIdx]) {
                return INVALID;
            }
            return (*sparsePages[pageIdx])[entityId % PAGE_SIZE];
        }

        inline void setDenseIndex(size_t entityId, size_t denseIdx) {
            size_t pageIdx = entityId / PAGE_SIZE;
            if (pageIdx >= sparsePages.size()) {
                sparsePages.resize(pageIdx + 1);
            }
            if (!sparsePages[pageIdx]) {
                sparsePages[pageIdx] = std::make_unique<Page>();
                sparsePages[pageIdx]->fill(INVALID);
            }
            (*sparsePages[pageIdx])[entityId % PAGE_SIZE] = denseIdx;
        }

    public:
        ComponentPool() = default;

        inline bool has(const Entity& entity) const {
            return getDenseIndex(entity.id) != INVALID;
        }

        inline T& get(const Entity& entity) {
            size_t idx = getDenseIndex(entity.id);
            if (idx == INVALID) {
                idx = data.size();
                setDenseIndex(entity.id, idx);
                data.emplace_back();
                denseToEntity.push_back(entity.id);
            }
            return data[idx];
        }

        inline std::vector<T>& getData() {
            return data;
        }

        inline void add(const Entity& entity, const T& component) {
            size_t idx = getDenseIndex(entity.id);
            if (idx != INVALID) {
                data[idx] = component;
                return;
            }
            setDenseIndex(entity.id, data.size());
            data.push_back(component);
            denseToEntity.push_back(entity.id);
        }

        inline void destroy(const Entity& entity) {
            size_t idx = getDenseIndex(entity.id);
            if (idx == INVALID) {
                return;
            }

            size_t lastIdx = data.size() - 1;
            if (idx != lastIdx) {
                size_t movedEntityId = denseToEntity[lastIdx];
                data[idx] = std::move(data[lastIdx]);
                denseToEntity[idx] = movedEntityId;
                setDenseIndex(movedEntityId, idx);
            }

            data.pop_back();
            denseToEntity.pop_back();
            setDenseIndex(entity.id, INVALID);
        }
    };

}