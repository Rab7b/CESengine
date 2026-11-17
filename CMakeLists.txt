cmake_minimum_required(VERSION 3.21)
project(CESengine VERSION 1.0.0 LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_library(ces_engine INTERFACE)
add_library(CESengine::ces_engine ALIAS ces_engine)

target_compile_features(ces_engine INTERFACE cxx_std_20)

target_include_directories(ces_engine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

option(CES_BUILD_EXAMPLES "Build engine examples" ON)
if(CES_BUILD_EXAMPLES AND PROJECT_IS_TOP_LEVEL)
    add_executable(test1 examples/test1.cpp)
    target_link_libraries(test1 PRIVATE ces_engine)

    find_package(SFML QUIET COMPONENTS graphics window system)
    if(SFML_FOUND)
        target_link_libraries(test1 PRIVATE sfml-graphics sfml-window sfml-system)
        message(STATUS "SFML found! Linked to test1.")
    else()
        message(WARNING "SFML not found on this system. Make sure SFML is installed if test1 requires graphics.")
    endif()
endif()
