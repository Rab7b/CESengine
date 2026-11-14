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

include(GNUInstallDirs)

install(DIRECTORY include/CESengine DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ces_engine
    EXPORT CESengineTargets
)

install(EXPORT CESengineTargets
    FILE CESengineTargets.cmake
    NAMESPACE CESengine::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/CESengine
)
