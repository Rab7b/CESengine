# `ces::` — Lightweight, Data-Oriented C++ ECS Engine

A high-performance, proprietary C++17/20 Entity Component System (ECS) designed around Data-Oriented Design (DOD). `ces::` eliminates dynamic heap allocation bottlenecks and pointer chasing, storing components in tightly packed contiguous memory buffers for maximum CPU cache efficiency and blazingly fast iteration speeds.

---

## 🚀 Key Features

* **Cache-Friendly Architecture:** Fixed contiguous memory storage using pre-allocated vector buffers for $O(1)$ component access and optimal CPU pre-fetching.
* **Zero Pointer Chasing:** Non-copyable objects (like SFML graphics, sprites, and shapes) are stored via `std::optional` inside contiguous memory using in-place construction (`.emplace()`), avoiding heap fragmentation and slow pointer dereferencing.
* **Safe Bitset Tracking:** Blazing-fast component mask matching using `std::bitset` for efficient entity filtering and life-cycle management.
* **Modern C++ Standard:** Built with C++17/20 practices, perfect forwarding, and RAII principles.
* **Seamless CMake Integration:** Drop-in library structure that cleanly propagates include paths and dependencies to dependent projects.

---

## 🛠 Project Structure

Here is how your project directory tree should look when integrating `ces::` into a game application:

```text
MyGame/                                <-- Your Game Project Root
│
├── CMakeLists.txt                     <-- Your Game's CMake Configuration
│
├── Engine/                            <-- The ces:: Engine Submodule / Folder
│   ├── CMakeLists.txt                 <-- Engine CMake File
│   ├── Registry.cpp                   <-- ECS Engine Implementation
│   ├── LICENSE                        <-- License File
│   └── include/
│       ├── Registry.h                 <-- Core Registry Class & API
│       └── types.h                    <-- Entity IDs & Component Types
│
└── src/
    └── main.cpp                       <-- Your Game's Entry Point
```

---

## ⚡ Integration & CMake Setup

Integrating `ces::` into your own game takes just a few lines of CMake code.

### 1. Engine Configuration (`Engine/CMakeLists.txt`)
Inside the `Engine/` folder, ensure your `CMakeLists.txt` is structured as follows:

```cmake
cmake_minimum_required(VERSION 3.16)
project(CesEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SFML dependency
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Engine static library target
add_library(ces_engine STATIC
    Registry.cpp
)

# Export alias namespace
add_library(ces::engine ALIAS ces_engine)

# Include paths propagation
target_include_directories(ces_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link SFML dependencies
target_link_libraries(ces_engine PUBLIC sfml-graphics sfml-window sfml-system)
```

### 2. Game Configuration (`CMakeLists.txt`)
At the root level of your game project (`MyGame/CMakeLists.txt`), link the engine using `add_subdirectory`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyAwesomeGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# 1. Bring in your engine source directory
add_subdirectory(Engine)

# 2. Define your game executable
add_executable(MyGame src/main.cpp)

# 3. Link your engine (automatically pulls in include paths & dependencies!)
target_link_libraries(MyGame PRIVATE ces::engine)
```

---

## 💻 Quickstart Usage Example

Here is a minimal example showing how to initialize the registry, spawn entities, attach components, and read component data in your game loop:

```cpp
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Registry.h"

int main() {
    ces::Registry registry;

    // Create a new entity
    ces::Entity player = registry.createEntity();

    // Assign transform & velocity components
    registry.setTransform(player, { 100.0f, 200.0f });
    registry.setVelocity(player, { 1.5f, 0.0f });

    // Assign visual component in-place without heap allocation or copying
    sf::Texture texture;
    // registry.assignVisual(player, texture);

    // Main game loop logic
    if (registry.isAlive(player)) {
        auto& pos = registry.getPosition(player);
        auto& vel = registry.getVelocity(player);

        // Update movement
        pos.x += vel.vx;
        pos.y += vel.vy;

        std::cout << "Player Position: (" << pos.x << ", " << pos.y << ")\n";
    }

    return 0;
}
```
