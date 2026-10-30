cmake_minimum_required(VERSION 3.16)
project(CesEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Request SFML 3.0 and use capital component names
find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_library(ces_engine STATIC
    Registry.cpp
)

add_library(ces::engine ALIAS ces_engine)

target_include_directories(ces_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Use the SFML 3 target namespace and capitalization
target_link_libraries(ces_engine PUBLIC SFML::Graphics SFML::Window SFML::System)