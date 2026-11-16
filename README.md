<div align="center">

<h1>🌌 CES Engine</h1>

<p>
  <strong>A lightning-fast, header-only C++20 Entity Component System (ECS) engine built from scratch.</strong>
</p>

<p>
  <code>C++20</code> &bull; <code>SFML 3.0</code> &bull; <code>CMake 3.20+</code> &bull; <code>Header-Only</code> &bull; <code>MIT License</code>
</p>

<blockquote>
  Designed for extreme performance, handling <b>1,000,000+ entities at 120+ FPS</b> using contiguous component memory pools and batched hardware rendering via SFML.
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
      <h3>🪐 Custom Physics</h3>
      Real-time N-body and vortex calculations, angular velocity, and boundary wrapping.
    </td>
    <td width="50%" valign="top">
      <h3>🛠️ Modern C++20</h3>
      Utilizes modern language standards for clean, expressive, and safe low-level systems programming.
    </td>
  </tr>
</table>

<br />

## 🚀 Quick Look: The Main Loop

Here is a sneak peek of how clean it is to set up a massive simulation using the engine's registry and systems:

```cpp
#include <include/ces.h>

int main() {
    auto& engine = ces::Engine::getInstance({1280, 720}, "My Game!", false, 1000.0f);

    engine.addSystem<ces::MovementSystem>();
    engine.addSystem<ces::DrawSystem>();

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
* **SFML 3.0** (Simple and Fast Multimedia Library)

</details>

<details open>
<summary><b>2. Linking with CMake (FetchContent)</b></summary>
<br />

To integrate **CESengine** into your C++20 project using CMake and `FetchContent`, add the following to your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.21)
project(YourGameName LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Declare and pull CESengine from GitHub via FetchContent
include(FetchContent)
FetchContent_Declare(
    CESengine
    GIT_REPOSITORY [https://github.com/Rab7b/CESengine.git](https://github.com/Rab7b/CESengine.git)
    GIT_TAG        main
)
FetchContent_MakeAvailable(CESengine)

# 2. Add your executable target
add_executable(YourGameName main.cpp)

# 3. Link CESengine to your target
target_link_libraries(YourGameName PRIVATE 
    CESengine::ces_engine
)
```

</details>

<br />

## 📊 Performance Benchmarks

Running on consumer hardware, the engine achieves buttery-smooth frame rates by keeping data cache-friendly:

<div align="center">

| Metric | Benchmark | Architecture Advantage |
| :--- | :---: | :--- |
| **Entity Count** | `1,000,000+` | Contiguous memory pool allocation |
| **Framerate** | `120+ FPS` | Single-threaded update & batch draw |
| **GPU Load** | `~5%` | Optimized GPU rendering `ces::VertexBuffer` |

</div>

<br />

## 📜 License

<div align="center">

Distributed under the **[MIT License](./LICENSE)**.  
<sub>Feel free to fork, experiment, and build cool things!</sub>

</div>
