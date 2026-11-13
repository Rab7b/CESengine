cmake_minimum_required(VERSION 3.21)
project(CESengine VERSION 1.0.0 LANGUAGES CXX)

# Only set global C++ standard defaults if this is the top-level project
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Define the engine as a header-only interface library
add_library(ces_engine INTERFACE)
add_library(CESengine::ces_engine ALIAS ces_engine)

# Enforce C++20 on any target that links against ces_engine
target_compile_features(ces_engine INTERFACE cxx_std_20)

# Expose include directories for both build-tree and installed usage
target_include_directories(ces_engine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
