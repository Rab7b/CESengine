#pragma once
#include "System.h"
#include <SFML/Window/Keyboard.hpp>
#include <unordered_set>

namespace ces {
    class KeyboardSystem : public ces::System {
    private:
        std::unordered_set<ces::Keyboard::Key> pressedKeys;

    public:
        using ces::System::System;

        void runIt(ces::Entity& en) override {
            pressedKeys.clear();

            for (int i = 0; i < static_cast<int>(ces::Keyboard::KeyCount); ++i) {
                ces::Keyboard::Key currentKey = static_cast<ces::Keyboard::Key>(i);
                if (ces::Keyboard::isKeyPressed(currentKey)) {
                    pressedKeys.insert(currentKey);
                }
            }
        }

        bool isKeyPressed(ces::Keyboard::Key key) const {
            return pressedKeys.find(key) != pressedKeys.end();
        }

        template<typename... Keys>
        bool isAnyKeyPressed(Keys... keys) const {
            return (isKeyPressed(keys) || ...);
        }

        const std::unordered_set<ces::Keyboard::Key>& getPressedKeys() const {
            return pressedKeys;
        }
    };
}