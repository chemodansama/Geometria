//
//  OpenGL.h
//
//  Created by Grigory Shabaganov on 23.07.2017.
//

#pragma once

#ifdef ANDROID
#ifdef GLES2
#include <GLES2/gl2.h>
#else
#include <GLES/gl.h>
#endif
#define glOrtho glOrthof
#elif __APPLE__
#include <OpenGL/gl3.h>
#elif defined(_WIN32)
#include <GL/glew.h>
#include <gl/gl.h>
#else
static_assert(false, "Failed to detect gl headers location")
#endif
