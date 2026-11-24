#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include "../Types/types.h"

namespace ces {

    template <typename T, std::uint32_t PAGE_SIZE = 4096>
    class ComponentPool {
    private:
        static constexpr std::uint32_t INVALID = static_cast<std::uint32_t>(maxEntities);
        using Page = std::array<std::uint32_t, PAGE_SIZE>;

        std::vector<std::unique_ptr<Page>> sparsePages;
        std::vector<T> data;
        std::vector<std::uint32_t> denseToEntity;

        inline std::uint32_t getDenseIndex(std::uint32_t entityId) const {
            std::uint32_t pageIdx = entityId / PAGE_SIZE;
            if (pageIdx >= sparsePages.size() || !sparsePages[pageIdx]) {
                return INVALID;
            }
            return (*sparsePages[pageIdx])[entityId % PAGE_SIZE];
        }

        inline void setDenseIndex(std::uint32_t entityId, std::uint32_t denseIdx) {
            std::uint32_t pageIdx = entityId / PAGE_SIZE;
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
            std::uint32_t idx = getDenseIndex(entity.id);
            if (idx == INVALID) {
                idx = static_cast<std::uint32_t>(data.size());
                setDenseIndex(entity.id, idx);
                data.emplace_back();
                denseToEntity.push_back(entity.id);
            }
            return data[idx];
        }

        inline const T& get(const Entity& entity) const {
            std::uint32_t idx = getDenseIndex(entity.id);
            assert(idx != INVALID && "Entity does not have this component!");
            return data[idx];
        }

        inline std::vector<T>& getData() {
            return data;
        }

        inline const std::vector<T>& getData() const {
            return data;
        }

        inline const std::vector<std::uint32_t>& getEntities() const {
            return denseToEntity;
        }

        inline void add(const Entity& entity, const T& component) {
            std::uint32_t idx = getDenseIndex(entity.id);
            if (idx != INVALID) {
                data[idx] = component;
                return;
            }
            setDenseIndex(entity.id, static_cast<std::uint32_t>(data.size()));
            data.push_back(component);
            denseToEntity.push_back(entity.id);
        }

        inline void destroy(const Entity& entity) {
            std::uint32_t idx = getDenseIndex(entity.id);
            if (idx == INVALID) {
                return;
            }

            std::uint32_t lastIdx = static_cast<std::uint32_t>(data.size() - 1);
            if (idx != lastIdx) {
                std::uint32_t movedEntityId = denseToEntity[lastIdx];
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
