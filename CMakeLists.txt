cmake_minimum_required(VERSION 3.15)
project(Geometria)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

option(GEOMETRIA_BUILD_SAMPLES "Build samples" ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/base")
add_subdirectory("${PROJECT_SOURCE_DIR}/render")
add_subdirectory("${PROJECT_SOURCE_DIR}/extension")

if (GEOMETRIA_BUILD_SAMPLES)
    add_subdirectory("${PROJECT_SOURCE_DIR}/samples")
endif()