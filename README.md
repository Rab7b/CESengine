<div align="center">

<h1>🌌 CES Engine</h1>

<p>
  <strong>A lightning-fast, header-only C++20 Entity Component System (ECS) engine built from scratch.</strong>
</p>

<p>
  <code>C++20</code> &bull; <code>SFML 3.0</code> &bull; <code>CMake 3.21+</code> &bull; <code>Header-Only</code> &bull; <code>MIT License</code>
</p>

<blockquote>
  Designed for extreme performance, handling <b>1,000,000+ entities at 150+ FPS</b> using contiguous component memory pools and batched hardware rendering via SFML.
</blockquote>

</div>

<br />

## ✨ Core Architecture & Features

<table>
  <tr>
    <td width="50%" valign="top">
      <h3>🧩 Header-Only ECS</h3>
      Clean, template-backed component storage (<code>Registry</code>, <code>ComponentPool</code>) with zero external bloat.
    </td>
    <td width="50%" valign="top">
      <h3>⚡ Batch Rendering</h3>
      Bypasses heavy draw calls by leveraging <code>ces::VertexBuffer</code> for single-draw rendering of massive particle counts.
    </td>
  </tr>
  <tr>
    <td width="50%" valign="top">
      <h3>🪐 Custom Physics & Parallelism</h3>
      Real-time N-body and vortex calculations with seamless OpenMP multi-threading support.
    </td>
    <td width="50%" valign="top">
      <h3>🛠️ Modern C++20</h3>
      Utilizes modern language standards for clean, expressive, and safe low-level systems programming.
    </td>
  </tr>
</table>

<br />

## 🚀 Quick Look: Creating Entities & Main Loop

Here is a sneak peek of how clean it is to spawn entities, attach components, and run a simulation:

```cpp
#include <ces.h>

int main() {
    //Create a game window
    auto& engine = ces::Engine::getInstance({1280.0f, 720.0f}, "My Game!", false, 1000.0f);

    //Required systems
    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>();

    //Get the registry
    auto& registry = engine.getRegistry();
    auto& transformPool = registry.getPool<ces::Transform>();
    auto& velocityPool = registry.getPool<ces::Velocity>();

    // Spawn an entity and attach components
    ces::Entity player = registry.createEntity();
    transformPool.add(player, {640.0f, 360.0f, 5.0f, 0.0f, ces::Color(100, 200, 255)});
    velocityPool.add(player, {50.0f, 0.0f});

    //Run the game loop
    while (engine.getWindow().isOpen()) {
        engine.run();
    }

    return 0;
}
```

<br />

## 📦 Setup & CMake Integration

<details open>
<summary><b>1. Requirements & Dependencies</b></summary>
<br />

* **C++20** compatible compiler (MSVC 2022/2026, Clang, or GCC)
* **CMake 3.21+**
* **SFML 3.0** (Automatically fetched via CMake)
* **OpenMP** *(Optional, for multi-threaded physics loops)*

</details>

<details open>
<summary><b>2. Building the Engine & Examples</b></summary>
<br />

Clone the repository and build using CMake. SFML 3.0.2 will be downloaded and linked automatically:

```cmake
cmake_minimum_required(VERSION 3.21)
project(CESEngine VERSION 1.0.0 LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

include(FetchContent)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/3.0.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(SFML)

find_package(OpenMP)

add_library(ces_engine INTERFACE)
add_library(CESEngine::ces_engine ALIAS ces_engine)

target_compile_features(ces_engine INTERFACE cxx_std_20)

target_include_directories(ces_engine INTERFACE
    \$<BUILD_INTERFACE:\({CMAKE_CURRENT_SOURCE_DIR}/include>\)<INSTALL_INTERFACE:include>
)

target_link_libraries(ces_engine INTERFACE 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(ces_engine INTERFACE OpenMP::OpenMP_CXX)
endif()

option(CES_BUILD_EXAMPLES "Build engine examples" ON)
if(CES_BUILD_EXAMPLES AND PROJECT_IS_TOP_LEVEL)
    add_executable(black_hole examples/test1.cpp)
    target_link_libraries(black_hole PRIVATE ces_engine)
    message(STATUS "SFML 3 successfully fetched and linked to CES Engine and examples.")
endif()
```

</details>

<br />

## 🎯 Motivation & Goals

<div align="center">

Most game engines today feel way too bloated and intimidating, so I'm building my own lightweight, easy-to-use alternative. Yes, I know it's ironic to call an "easy" tool the **Complex Entity System**, but keeping things simple is the whole point. I'd really appreciate any feedback or support on what I have so far!

</div>

## 📊 Performance Benchmarks

Running the **1,000,001-particle Black Hole simulation** on consumer hardware, the engine achieves smooth frame rates by keeping data cache-friendly:

<div align="center">

| Metric | Benchmark | Architecture Advantage |
| :--- | :---: | :--- |
| **Entity Count** | `1,000,000+` | Contiguous memory pool allocation |
| **Framerate** | `150+ FPS` | OpenMP parallel updates & batch draw |
| **GPU Load** | `~7%` | Single draw call via `ces::VertexBuffer` |

</div>

<br />

## 📜 License

<div align="center">

Distributed under the **[MIT License](./LICENSE)**.  
<sub>Feel free to fork, experiment, and build cool things!</sub>

</div>
