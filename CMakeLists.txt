cmake_minimum_required(VERSION 3.15)
project(Geometria)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

option(GEOMETRIA_BUILD_SAMPLES "Build samples" ON)
option(GEOMETRIA_BUILD_RENDER "Build render module" ON)
option(GEOMETRIA_BUILD_EXTENSION "Build extension module" ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/base")

if (GEOMETRIA_BUILD_RENDER)
    add_subdirectory("${PROJECT_SOURCE_DIR}/render")
endif()

if (GEOMETRIA_BUILD_EXTENSION)
    add_subdirectory("${PROJECT_SOURCE_DIR}/extension")
endif()

if (GEOMETRIA_BUILD_SAMPLES)
    add_subdirectory("${PROJECT_SOURCE_DIR}/samples")
endif()