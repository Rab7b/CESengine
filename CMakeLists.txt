cmake_minimum_required(VERSION 3.16)
project(CesEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_library(ces_engine STATIC
    Registry.cpp
    Registry.h
    Engine.h
    System.h
    DrawSystem.h
    MovementSystem.h
    KeyboarSystem.h
    types.h
)

add_library(ces::engine ALIAS ces_engine)

target_include_directories(ces_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(ces_engine PUBLIC sfml-graphics sfml-window sfml-system)
