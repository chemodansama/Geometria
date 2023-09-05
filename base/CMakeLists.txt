cmake_minimum_required(VERSION 3.15)
project(GeometriaBase)

option(GEOMETRIA_BUILD_TESTS "Build tests" ON)
option(GEOMETRIA_STATIC_RUNTIME "Static MSVC runtime" OFF)

set(
    SOURCES
    "source/math/Plane.cpp"

    "source/assets.cpp"
    "source/debug.cpp"
    "source/easings.cpp"
    "source/Observable.cpp"
    "source/path.cpp"
    "source/utils.cpp"
    "source/Weak.cpp"
)

set(
    HEADERS
    "include/gmt/math/2d.h"
    "include/gmt/math/Plane.h"

    "include/gmt/assets.h"
    "include/gmt/debug.h"
    "include/gmt/easings.h"
    "include/gmt/Event.h"
    "include/gmt/gmt.h"
    "include/gmt/Observable.h"
    "include/gmt/path.h"
    "include/gmt/utils.h"
    "include/gmt/Weak.h"
)

set(all_code_files
    ${SOURCES}
    ${HEADERS}
)

add_library(GeometriaBase STATIC ${all_code_files})
add_library(geometria::base ALIAS GeometriaBase)

include_directories("${PROJECT_BINARY_DIR}")
target_include_directories(GeometriaBase PUBLIC "${PROJECT_SOURCE_DIR}/include")

foreach(SOURCE IN LISTS all_code_files)
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    string(REPLACE "/" "\\" SOURCE_GROUP "${SOURCE_PATH}")
    source_group("${SOURCE_GROUP}" FILES "${SOURCE}")
endforeach()

target_compile_features(GeometriaBase PRIVATE cxx_std_17)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    if (GEOMETRIA_STATIC_RUNTIME)
        set_property(TARGET GeometriaBase PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    target_link_libraries(GeometriaBase PUBLIC Dbghelp.lib)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/glm-0.9.9.8")
target_link_libraries(GeometriaBase PUBLIC glm::glm)

add_subdirectory("${PROJECT_SOURCE_DIR}/thirdparty/fmt-10.1.1")
target_link_libraries(GeometriaBase PUBLIC fmt::fmt-header-only)

if (GEOMETRIA_BUILD_TESTS)
    if (GEOMETRIA_STATIC_RUNTIME)
        message(FATAL_ERROR "Can't have static runtime with Gtests. Either turn off static runtime or turn off tests.")
    endif()
    add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
endif()