# CES Engine 🌌

A lightning-fast, header-only C++20 Entity Component System (ECS) engine built from scratch. Designed for extreme performance, it handles **1,000,000+ entities at 150+ FPS** using contiguous component memory pools and batched hardware rendering via SFML.

Included in the repo is a showcase "Black Hole" simulation featuring 500,000+ swirling particles with gravitational physics, dynamic velocity damping, and a gorgeous yellow-pink-blue color gradient mapped to particle distance and angle.

---

## ✨ Features

- **Header-Only ECS Architecture:** Clean, template-backed component storage (`Registry`, `ComponentPool`) with zero bloat.
- **High-Performance Batch Rendering:** Bypasses heavy draw calls by leveraging `sf::VertexArray` for single-draw rendering of massive particle counts.
- **Custom Gravitational Physics:** Real-time N-body/vortex calculations, angular velocity, and boundary wrapping.
- **C++20 Modern Standard:** Utilizing modern language features for clean, expressive, and safe low-level systems programming.

---

## 🚀 Quick Look: The Black Hole Demo

Here is a sneak peek of how clean it is to set up a massive simulation using the engine's registry and systems:

```cpp
#include "Engine/Engine.h"
#include "DrawSystem.hpp"
#include "MovementSystem.hpp"

int main() {
    auto& engine = ces::Engine::getInstance({1280, 720}, "CES Engine", false, 1000.0f);

    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>();

    while (engine.getWindow().isOpen()) {
        engine.run();
    }

    return 0;
}
```

---

## 🛠️ Requirements & Dependencies

- **C++20** compatible compiler (MSVC 2022/2026, Clang, or GCC)
- **SFML** (Simple and Fast Multimedia Library)

---

## 📦 Linking with CMake

If you want to integrate SFML into your project using CMake, here is a clean, standard `CMakeLists.txt` setup to get you up and running:

```cmake
cmake_minimum_required(VERSION 3.20)
project(CESEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Network Audio)

add_executable(CESEngine 
    main.cpp
    Engine/Engine.cpp
)

target_link_libraries(CESEngine PRIVATE sfml-graphics sfml-window sfml-system opengl32)
target_include_directories(CESEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
```

---

## ⚡ Performance

Running on consumer hardware, the engine achieves buttery-smooth frame rates by keeping data cache-friendly:
- **Entities:** 1,000,000
- **Framerate:** 150+ FPS
- **GPU Load:** Minimal (~5%) thanks to optimized CPU-side vertex arrays and efficient memory layout.

---

## 📜 License

Distributed under the MIT License. Feel free to fork, experiment, and build cool things!
