cmake_minimum_required(VERSION 3.21)
project(CESengine VERSION 1.0.0 LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Explicitly point to Ubuntu's multiarch CMake path for SFML
find_package(SFML REQUIRED COMPONENTS graphics window system 
    HINTS 
        /usr/lib/x86_64-linux-gnu/cmake/SFML
        /usr/lib/cmake/SFML 
        /usr/local/lib/cmake/SFML
)

add_library(ces_engine INTERFACE)
add_library(CESengine::ces_engine ALIAS ces_engine)

target_compile_features(ces_engine INTERFACE cxx_std_20)

target_include_directories(ces_engine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(ces_engine INTERFACE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

option(CES_BUILD_EXAMPLES "Build engine examples" ON)
if(CES_BUILD_EXAMPLES AND PROJECT_IS_TOP_LEVEL)
    add_executable(test1 examples/test1.cpp)
    target_link_libraries(test1 PRIVATE ces_engine)
    message(STATUS "SFML found successfully and linked to CESengine and examples.")
endif()
