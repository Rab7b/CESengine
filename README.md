# SandboxGame & CES Engine

A lightweight, high-performance C++20 Entity Component System (ECS) game engine integrated with SFML 3.0 for 2D graphics rendering. Built with Visual Studio, CMake, and `vcpkg`, featuring automated engine dependency management via CMake `FetchContent`.

---

## 🚀 Setup & Build Instructions

### Prerequisites
* **C++20 Compiler** (MSVC, GCC, or Clang)
* **CMake 3.16+** & **Ninja**
* **vcpkg** (C++ Package Manager)

### Installation
1. Install **SFML 3.0** via `vcpkg`:
   ```bash
   vcpkg install sfml
   ```
2. Open the project folder in Visual Studio or configure it via terminal:
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
   cmake --build build
   ```

> **Note:** The `ces_engine` core library is automatically fetched and compiled from GitHub during CMake generation.

---

## ⚙️ CMake Configuration (`CMakeLists.txt`)

```cmake
cmake_minimum_required(VERSION 3.16)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("SandboxGame")

# Find SFML package via vcpkg
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

# 1. Pull the engine straight from GitHub
include(FetchContent)
FetchContent_Declare(
    ces_engine
    GIT_REPOSITORY https://github.com/Rab7b/CESengine.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(ces_engine)

# 2. Add your game executable
add_executable(SandboxGame "SandboxGame.cpp" "SandboxGame.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SandboxGame PROPERTY CXX_STANDARD 20)
endif()

# 3. Link engine and SFML to the game
target_link_libraries(SandboxGame PRIVATE ces_engine SFML::Graphics SFML::Window SFML::System)
```

---

## 📖 API Documentation

### `ces::Engine`
The central manager class (Singleton pattern).

* `static Engine& getInstance()`: Returns the global engine instance.
* `Registry& getRegistry()`: Returns a reference to the main ECS registry instance.
* `sf::RenderWindow& getWindow()`: Returns a reference to the active SFML render window.
* `bool isWindowOpen()`: Returns `true` if the main window remains active.
* `void run()`: Executes a single frame cycle across registered engine systems.
* `template<typename T, typename... Args> void addSystem(Args&&... args)`: Registers a system. *Injects `Registry&` automatically as the first parameter to the constructor.*

### `ces::Registry`
Manages entity creation, component assignment, and queries.

* `Entity createEntity()`: Instantiates and returns a new entity identifier.
* `void destroyEntity(Entity entity)`: Recycles an entity ID and clears associated component data.
* `bool isAlive(Entity entity) const`: Checks if an entity ID is currently active.
* `void setTransform(Entity entity, const Position& t)`: Assigns a spatial transform component.
* `void setVelocity(Entity entity, const Velocity& v)`: Assigns a velocity component.
* `template<typename ShapeType, typename... Args> void assignVisual(Entity entity, Args&&... args)`: Constructs an SFML visual shape (e.g. `sf::RectangleShape`) using variadic arguments.
* `Position& getPosition(Entity entity)`: Fetches an entity's position data.
* `Velocity& getVelocity(Entity entity)`: Fetches an entity's velocity data.
* `Visual& getVisual(Entity entity)`: Fetches an entity's visual variant data wrapper.

### `ces::DrawSystem`
* Built-in system extending `ces::System`.
* Uses `std::visit` on standard component variants to render entity shapes (`sf::RectangleShape`, etc.) relative to their transform components inside the main window.

---

## 💻 Example Code (`SandboxGame.cpp`)

```cpp
#include "SandboxGame.h"
#include <Engine.h>

using namespace std;

int main()
{
	ces::Engine& engine = ces::Engine::getInstance();
	ces::Registry& reg = engine.getRegistry();

	// Add the draw system (engine automatically injects 'reg' as 1st argument)
	engine.addSystem<ces::DrawSystem>(engine.getWindow());

	// Create a test entity and assign components
	ces::Entity e1 = reg.createEntity();

	reg.assignVisual<sf::RectangleShape>(e1, sf::Vector2f(50.f, 50.f));
	reg.setTransform(e1, { 300.f, 40.f });
	reg.setVelocity(e1, { 0.f, 0.f });
	
	// Main game loop
	while (engine.isWindowOpen()) {
		engine.run();
	}

	return 0;
}
```