cmake_minimum_required(VERSION 3.22.1)

project("Samples")

add_library(Samples SHARED Samples.cpp)

target_compile_features(Samples PRIVATE cxx_std_17)

set(GEOMETRIA_BUILD_SAMPLES OFF)
set(GEOMETRIA_BUILD_TESTS OFF)
set(GEOMETRIA_BUILD_RENDER OFF)
set(GEOMETRIA_BUILD_EXTENSION OFF)
set(GEOMETRIA_STATIC_RUNTIME ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/../../../../../../" "${PROJECT_SOURCE_DIR}/.geometria")
target_link_libraries(Samples geometria::base)

find_library(log-lib log)
target_link_libraries(Samples ${log-lib})
