#pragma once

#include "gmt/render/OpenGL.h"

namespace gmt
{

class VertexArray
{
public:
    VertexArray(void) { glGenVertexArrays(1, &vao_); }
    VertexArray(VertexArray&) = delete;
    VertexArray(VertexArray&& v) noexcept : vao_{ v.vao_ } { v.vao_ = 0; }
    VertexArray &operator=(VertexArray&) = delete;
    VertexArray &operator=(VertexArray&& v) { release(); vao_ = v.vao_; v.vao_ = 0; return *this; }
    ~VertexArray(void) { release(); }

    operator GLuint() const { return vao_; }
    void bind() const { glBindVertexArray(vao_); }
    static void unbind() { glBindVertexArray(0); }

private:
    GLuint vao_;

    void release()
    {
        if (vao_) {
            glDeleteVertexArrays(1, &vao_);
        }
    }
};

}
