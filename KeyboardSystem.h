#pragma once
#include "System.h"
#include <SFML/Window/Keyboard.hpp>

namespace ces {
    class KeyboardSystem : public ces::System {
    private:
        ces::Keyboard::Key key;

    public:
        using ces::System::System;

        void runIt(ces::Entity& en) override {
            this->key = ces::Keyboard::Key::Unknown;

            for (int i = 0; i < static_cast<int>(ces::Keyboard::KeyCount); ++i) {
                ces::Keyboard::Key currentKey = static_cast<ces::Keyboard::Key>(i);
                if (ces::Keyboard::isKeyPressed(currentKey)) {
                    this->key = currentKey;
                }
            }
        }

        ces::Keyboard::Key getPressedKey() {
            return this->key;
        }
    };
}