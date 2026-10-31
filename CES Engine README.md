# CES Engine

`ces_engine` is a lightweight, modular **Entity Component System (ECS)** game engine written in **C++17** and built on top of **SFML 3.0.2**. Designed with a focus on streamlined developer workflows and fast iteration, it provides a clean structure for building 2D games rapidly.

---

## 🚀 Key Features

* **Custom ECS Architecture:** Modular registry managing entities, bitset flags, and flexible component storage leveraging `std::variant`.
* **SFML 3.0.2 Integration:** Native integration with SFML 3.0 graphics, windowing, and system components.
* **Template System Pipeline:** Decoupled system management through a templated engine registration workflow (`addSystem`).
* **Seamless Build Workflow:** Optimized for CMake integration via `FetchContent` alongside local directory override support (`FETCHCONTENT_SOURCE_DIR_CES_ENGINE`).

---

## 🛠️ Project Structure

```text
CESengine/
├── CMakeLists.txt        # Primary build configuration
├── README.md             # Project documentation
├── Engine.h              # Main engine manager class
├── Registry.h            # ECS entity and component registry
├── System.h              # Base class for engine systems
├── DrawSystem.h          # Built-in SFML visual rendering system
└── types.h               # Core ECS type definitions and variant wrappers
```

---

## 📦 Integrating into Your Project

### Integrating via CMake (`FetchContent`)

You can pull and build `ces_engine` directly inside your sandbox application. Add the following configuration to your project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(SandboxGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: Override remote fetch during local engine development
# set(FETCHCONTENT_SOURCE_DIR_CES_ENGINE "E:/Engine")

include(FetchContent)
FetchContent_Declare(
    ces_engine
    GIT_REPOSITORY https://github.com/Rab7b/CESengine.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(ces_engine)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(SandboxGame main.cpp)

target_link_libraries(SandboxGame PRIVATE 
    ces_engine
    SFML::Graphics 
    SFML::Window 
    SFML::System
)
```

---

## 💻 Quick Start Example

Here is a minimal example demonstrating entity creation and component assignment:

```cpp
#include "Engine.h"
#include "DrawSystem.h"

int main() {
    ces::Engine engine(800, 600, "CES Engine Sandbox");

    // Register visual draw system pass
    engine.addSystem<ces::DrawSystem>(engine.getWindow());

    auto& registry = engine.getRegistry();

    // Create an entity and attach transform and visual components
    ces::Entity entity = registry.createEntity();
    registry.setTransform(entity, { 400.f, 300.f });
    registry.setVelocity(entity, { 100.f, 50.f });

    // Assign an SFML shape component stored inside the Visual variant
    registry.assignVisual<sf::CircleShape>(entity, 25.f);

    // Main game loop
    while (engine.isOpen()) {
        engine.pollEvents();
        engine.clear(sf::Color(20, 20, 30));
        
        // System updates are handled inside the engine update cycle
        
        engine.display();
    }

    return 0;
}
```

---

## 📄 License

Created by **Rab7b**. Distributed under the MIT License. Feel free to modify and build upon it!