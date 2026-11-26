#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>
#include "../Types/types.h"

namespace ces {

    struct IComponentPool {
        virtual ~IComponentPool() = default;
        virtual void destroy(const Entity& entity) = 0;
    };

    template <typename T, size_t PAGE_SIZE = 4096>
    class ComponentPool : public IComponentPool {
    private:
        using SlotList = std::vector<size_t>;
        using Page = std::array<SlotList, PAGE_SIZE>;

        std::vector<std::unique_ptr<Page>> sparsePages;
        std::vector<T> data;
        std::vector<size_t> denseToEntity;

        inline SlotList* getSlots(size_t entityId) {
            size_t pageIdx = entityId / PAGE_SIZE;
            if (pageIdx >= sparsePages.size() || !sparsePages[pageIdx]) {
                return nullptr;
            }
            return &(*sparsePages[pageIdx])[entityId % PAGE_SIZE];
        }

        inline const SlotList* getSlots(size_t entityId) const {
            size_t pageIdx = entityId / PAGE_SIZE;
            if (pageIdx >= sparsePages.size() || !sparsePages[pageIdx]) {
                return nullptr;
            }
            return &(*sparsePages[pageIdx])[entityId % PAGE_SIZE];
        }

        inline SlotList& getOrCreateSlots(size_t entityId) {
            size_t pageIdx = entityId / PAGE_SIZE;
            if (pageIdx >= sparsePages.size()) {
                sparsePages.resize(pageIdx + 1);
            }
            if (!sparsePages[pageIdx]) {
                sparsePages[pageIdx] = std::make_unique<Page>();
            }
            return (*sparsePages[pageIdx])[entityId % PAGE_SIZE];
        }

        inline void removeSingleDenseIndex(size_t idx) {
            size_t ownerId = denseToEntity[idx];
            size_t lastIdx = data.size() - 1;

            if (idx != lastIdx) {
                size_t movedEntityId = denseToEntity[lastIdx];
                data[idx] = std::move(data[lastIdx]);
                denseToEntity[idx] = movedEntityId;

                if (SlotList* movedSlots = getSlots(movedEntityId)) {
                    for (size_t& slotIdx : *movedSlots) {
                        if (slotIdx == lastIdx) {
                            slotIdx = idx;
                            break;
                        }
                    }
                }
            }

            data.pop_back();
            denseToEntity.pop_back();

            if (SlotList* ownerSlots = getSlots(ownerId)) {
                auto it = std::find(ownerSlots->begin(), ownerSlots->end(), idx);
                if (it != ownerSlots->end()) {
                    ownerSlots->erase(it);
                }
            }
        }

    public:
        ComponentPool() = default;

        inline bool has(const Entity& entity) const {
            const SlotList* slots = getSlots(entity.id);
            return slots != nullptr && !slots->empty();
        }

        inline size_t count(const Entity& entity) const {
            const SlotList* slots = getSlots(entity.id);
            return slots ? slots->size() : 0;
        }

        inline const SlotList* getDenseIndices(const Entity& entity) const {
            return getSlots(entity.id);
        }

        inline T& get(const Entity& entity, size_t instanceIndex = 0) {
            SlotList& slots = getOrCreateSlots(entity.id);
            if (instanceIndex >= slots.size()) {
                size_t idx = data.size();
                slots.push_back(idx);
                data.emplace_back();
                denseToEntity.push_back(entity.id);
                return data[idx];
            }
            return data[slots[instanceIndex]];
        }

        inline std::vector<T>& getData() {
            return data;
        }

        inline void add(const Entity& entity, const T& component) {
            SlotList& slots = getOrCreateSlots(entity.id);
            size_t idx = data.size();
            slots.push_back(idx);
            data.push_back(component);
            denseToEntity.push_back(entity.id);
        }

        inline void add(const Entity& entity, T&& component) {
            SlotList& slots = getOrCreateSlots(entity.id);
            size_t idx = data.size();
            slots.push_back(idx);
            data.push_back(std::move(component));
            denseToEntity.push_back(entity.id);
        }

        inline void destroy(const Entity& entity) override {
            SlotList* slots = getSlots(entity.id);
            if (!slots || slots->empty()) {
                return;
            }

            while (!slots->empty()) {
                size_t idx = slots->back();
                removeSingleDenseIndex(idx);
            }
        }
    };

}
