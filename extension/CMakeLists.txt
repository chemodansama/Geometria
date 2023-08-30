cmake_minimum_required(VERSION 3.15)
project(GeometriaExtension)

option(GEOMETRIA_STATIC_RUNTIME "Static MSVC runtime" OFF)

include_directories("${PROJECT_BINARY_DIR}")

set(
    SOURCES
    "source/GeometriaEx.cpp"
    "include/gmt/GeometriaEx.h"
)
add_library(GeometriaExtension STATIC ${SOURCES})
add_library(geometria::extension ALIAS GeometriaExtension)

foreach(SOURCE IN LISTS SOURCES)
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    string(REPLACE "/" "\\" SOURCE_GROUP "${SOURCE_PATH}")
    source_group("${SOURCE_GROUP}" FILES "${SOURCE}")
endforeach()

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    if (GEOMETRIA_STATIC_RUNTIME)
        set_property(TARGET GeometriaExtension PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

target_compile_features(GeometriaExtension PRIVATE cxx_std_17)

find_package(PNG REQUIRED 16)
target_link_libraries(GeometriaExtension PUBLIC PNG::PNG)

target_link_libraries(GeometriaExtension PUBLIC geometria::base)

target_include_directories(GeometriaExtension PUBLIC "${PROJECT_SOURCE_DIR}/include")
