# 🎮 CES Sandbox Game & Engine

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-square&logo=c%2B%2B" alt="C++20">
  <img src="https://img.shields.io/badge/SFML-3.0.2-brightgreen.svg?style=flat-square&logo=sfml" alt="SFML 3.0">
  <img src="https://img.shields.io/badge/Architecture-ECS-orange.svg?style=flat-square" alt="ECS Architecture">
  <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20MSVC-lightgrey.svg?style=flat-square&logo=windows" alt="Windows Platform">
</p>

A lightning-fast, lightweight 2D sandbox game powered by a custom **Entity Component System (ECS)** engine written completely from first principles in **C++20** and rendered with **SFML 3.0**.

---

## ✨ Key Features

- **Custom ECS Architecture:** Built around a type-erased registry (`registry.has<T>()`, `registry.get<T>()`) eliminating heavy external framework bloat.
- **Modular System Pipeline:** Decoupled logic handlers for keyboard input, physics/movement, and multi-shape rendering (`sf::CircleShape`, `sf::RectangleShape`).
- **Variant-Based Visuals:** Flexible component storage using `std::variant` to support diverse renderable entities seamlessly.
- **Modern CMake Workflow:** Effortless dependency fetching and linking via CMake `FetchContent` and `vcpkg`.

---

## 📂 Project Structure

```text
SandboxGame/
├── CMakeLists.txt         # Project configuration & FetchContent setup
├── SandboxGame.cpp        # Main application entry point & entity loop
├── SandboxGame.h          # Game header declarations & configuration
└── README.md              # Project documentation
```

---

## 🛠️ Core API & Architecture

### 1. Types & Components (`types.h`)

All components are standard data structures. Visual representation relies on `std::variant` for runtime flexibility without runtime inheritance overhead:

```cpp
namespace ces {
    using Entity = uint32_t;
    inline constexpr Entity invalidEntity = 0xFFFFFFFF;

    struct Position {
        float x{0.0f}, y{0.0f};
    };

    struct Velocity {
        float vx{0.0f}, vy{0.0f};
    };

    struct Visual {
        std::variant<std::monostate, sf::CircleShape, sf::RectangleShape, sf::ConvexShape> it;
    };
}
```

### 2. Base System Interface (`System.h`)

All engine systems derive from `ces::System` and receive a reference to the active `Registry`:

```cpp
namespace ces {
    class System {
    protected:
        ces::Registry& reg;

    public:
        System(ces::Registry& r) : reg(r) {}
        virtual ~System() = default;
        virtual void runIt(ces::Entity& en) = 0;
    };
}
```

### 3. Registry Methods (`ces::Registry`)

* `Entity createEntity()` — Spawns a new entity ID.
* `template <typename T, typename... Args> void assign(Entity e, Args&&... args)` — Attaches a component to an entity.
* `template <typename T> bool has(Entity e)` — Checks for component existence.
* `template <typename T> T& get(Entity e)` — Retrieves reference to component `T`.

---

## 💡 Code Example

Here is the complete game main loop initializing the engine, registering systems, creating a player entity, and handling real-time WASD movement:

```cpp
#include <Engine.h>
#include <KeyboardSystem.h>
#include <MovementSystem.h>
#include <DrawSystem.h>
#include <types.h>

int main() {
    // 1. Initialize engine singleton (800x600, VSync ON, 60 FPS)
    auto& engine = ces::Engine::getInstance(800, 600, "CES Sandbox Game", true, 60);

    // 2. Register core systems
    engine.addSystem<ces::KeyboardSystem>();
    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>(engine.getWindow());

    // 3. Create player entity
    auto player = engine.getRegistry().createEntity();

    engine.getRegistry().assign<ces::Position>(player, 400.0f, 300.0f);
    engine.getRegistry().assign<ces::Velocity>(player, 0.0f, 0.0f);

    ces::RectangleShape rect({ 50.0f, 50.0f });
    rect.setFillColor(ces::Color::Green);
    rect.setOrigin({ 25.0f, 25.0f }); 

    engine.getRegistry().assign<ces::Visual>(player, rect);

    // 4. Main Game Loop
    while (engine.isWindowOpen()) {
        engine.run();

        // Reset player velocity each frame
        engine.getRegistry().get<ces::Velocity>(player).vx = 0.00f;
        engine.getRegistry().get<ces::Velocity>(player).vy = 0.00f;

        // Process WASD Input
        if (engine.getSystem<ces::KeyboardSystem>()->isKeyPressed(ces::Keyboard::Key::W)) {
            engine.getRegistry().get<ces::Velocity>(player).vy = -5.00f;
        }
        if (engine.getSystem<ces::KeyboardSystem>()->isKeyPressed(ces::Keyboard::Key::S)) {
            engine.getRegistry().get<ces::Velocity>(player).vy = 5.00f;
        }
        if (engine.getSystem<ces::KeyboardSystem>()->isKeyPressed(ces::Keyboard::Key::A)) {
            engine.getRegistry().get<ces::Velocity>(player).vx = -5.00f;
        }
        if (engine.getSystem<ces::KeyboardSystem>()->isKeyPressed(ces::Keyboard::Key::D)) {
            engine.getRegistry().get<ces::Velocity>(player).vx = 5.00f;
        }
    }

    return 0;
}
```

---

## ⚙️ CMake Configuration (`CMakeLists.txt`)

Dependencies are fetched directly via `FetchContent` and resolved with `vcpkg`:

```cmake
cmake_minimum_required(VERSION 3.16)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("SandboxGame")

# Find SFML package via vcpkg
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# Pull the engine straight from GitHub
include(FetchContent)
FetchContent_Declare(
    ces_engine
    GIT_REPOSITORY https://github.com/Rab7b/CESengine.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(ces_engine)

# Add game executable
add_executable(SandboxGame "SandboxGame.cpp" "SandboxGame.h")
set_property(TARGET SandboxGame PROPERTY CXX_STANDARD 20)

# Link targets
target_link_libraries(SandboxGame PRIVATE ces_engine SFML::Graphics SFML::Window SFML::System)
```

---

## 🚀 Getting Started

### Prerequisites
Make sure your development environment includes:
* **Visual Studio 2026** (with the *Desktop Development with C++* workload)
* **CMake** (v3.16+)
* **Ninja** build system
* **vcpkg** package manager (`vcpkg install sfml:x64-windows`)

### Quick Build & Run (Command Line)
Open your Developer Command Prompt or terminal in the project root and run:

```cmd
cmake -B out/build/x64-debug -S . -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE="Debug"
cmake --build out/build/x64-debug
```

### Build via Visual Studio
1. Open the project folder containing `CMakeLists.txt` directly in Visual Studio.
2. Let CMake auto-configure the `x64-Debug` environment.
3. Select **SandboxGame.exe** as your startup target and press **F5** to build and run!

---

## 🤝 Community & Contributing

We love community feedback and contributions! Whether you're optimizing core ECS memory pools, adding new systems, or fixing bugs:
1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

---

## 📜 License

Distributed under the MIT License. See `LICENSE` for more information.
