cmake_minimum_required(VERSION 3.20)
project(CESengine VERSION 1.0.0 LANGUAGES CXX)

# Force C++20 across all compilers (MSVC, GCC, Clang)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define the engine as a header-only interface library
add_library(ces_engine INTERFACE)

# Expose your include directory so headers can be included globally
target_include_directories(ces_engine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
