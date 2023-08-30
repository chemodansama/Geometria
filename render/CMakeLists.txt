cmake_minimum_required(VERSION 3.15)
project(GeometriaRender)

option(GEOMETRIA_STATIC_RUNTIME "Static MSVC runtime" OFF)

set(
    SOURCES
    "source/Context.cpp"
    "source/Framebuffer.cpp"
    "source/Frustum.cpp"
    "source/InputLayout.cpp"
    "source/Program.cpp"
    "source/Texture.cpp"
    "source/VertexArray.cpp"
    "source/VertexBuffer.cpp"
)

set(
    HEADERS
    "include/gmt/render/Context.h"
    "include/gmt/render/Framebuffer.h"
    "include/gmt/render/Frustum.h"
    "include/gmt/render/InputLayout.h"
    "include/gmt/render/OpenGL.h"
    "include/gmt/render/Program.h"
    "include/gmt/render/Texture.h"
    "include/gmt/render/VertexArray.h"
    "include/gmt/render/VertexBuffer.h"
)

set(all_code_files
    ${SOURCES}
    ${HEADERS}
)

add_library(GeometriaRender STATIC ${all_code_files})
add_library(geometria::render ALIAS GeometriaRender)

include_directories("${PROJECT_BINARY_DIR}")
target_include_directories(GeometriaRender PUBLIC "${PROJECT_SOURCE_DIR}/include")

foreach(SOURCE IN LISTS all_code_files)
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    string(REPLACE "/" "\\" SOURCE_GROUP "${SOURCE_PATH}")
    source_group("${SOURCE_GROUP}" FILES "${SOURCE}")
endforeach()

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    if (GEOMETRIA_STATIC_RUNTIME)
        set_property(TARGET GeometriaRender PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

target_compile_features(GeometriaRender PRIVATE cxx_std_17)

# frameworks go here
if (APPLE) 
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)

    target_link_libraries(GeometriaRender PUBLIC ${COCOA_LIBRARY})
    target_link_libraries(GeometriaRender PUBLIC ${IOKIT_LIBRARY})
    target_link_libraries(GeometriaRender PUBLIC ${COREVIDEO_LIBRARY})
    target_link_libraries(GeometriaRender PUBLIC ${OPENGL_LIBRARY})
    target_include_directories(GeometriaRender PUBLIC "/usr/local/include")
endif()

# OpenGL is deprecated on OSX, silence warning
if (APPLE)
    target_compile_definitions(GeometriaRender PRIVATE GL_SILENCE_DEPRECATION)
endif()

find_package(GLEW REQUIRED)
target_link_libraries(GeometriaRender PUBLIC GLEW::GLEW)

if (WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM_TARGET "x64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(PLATFORM_TARGET "x68")
    endif()

    target_link_libraries(GeometriaRender PUBLIC opengl32.lib)
endif()

target_link_libraries(GeometriaRender PUBLIC geometria::base)
