cmake_minimum_required(VERSION 3.15)
project(Samples)

option(GEOMETRIA_STATIC_RUNTIME "Static MSVC runtime" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

include_directories("${PROJECT_BINARY_DIR}")

set(
    SOURCES
    "source/Scenes/InstancingScene.cpp"
    "source/Scenes/InstancingScene.h"
    "source/Scenes/PostprocessScene.cpp"
    "source/Scenes/PostprocessScene.h"

    "source/Common.cpp"
    "source/Common.h"
    "source/Config.cpp"
    "source/Config.h"
    "source/GlfwContext.cpp"
    "source/GlfwContext.h"
    "source/main.cpp"
    "source/SampleMesh.cpp"
    "source/SampleMesh.h"
    "source/ShaderTypes.cpp"
    "source/ShaderTypes.h"
)
add_executable(Samples ${SOURCES})
target_sources(Samples PUBLIC "${PROJECT_SOURCE_DIR}/assets")
foreach(SOURCE IN LISTS SOURCES)
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    string(REPLACE "/" "\\" SOURCE_GROUP "${SOURCE_PATH}")
    source_group("${SOURCE_GROUP}" FILES "${SOURCE}")
endforeach()

target_compile_features(Samples PRIVATE cxx_std_17)

# frameworks go here
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(Samples ${COCOA_LIBRARY})
    target_link_libraries(Samples ${IOKIT_LIBRARY})
    target_link_libraries(Samples ${COREVIDEO_LIBRARY})
    target_include_directories(Samples PUBLIC "/usr/local/include")
endif()

target_link_libraries(Samples geometria::base)
target_link_libraries(Samples geometria::render)
target_link_libraries(Samples geometria::extension)

target_include_directories(Samples PRIVATE "${PROJECT_SOURCE_DIR}/source")


find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Samples glfw)

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    if (GEOMETRIA_STATIC_RUNTIME)
        set_property(TARGET Samples PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    set_property(TARGET Samples PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Samples>)
endif()

add_custom_command(
        TARGET Samples POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Samples>/assets)