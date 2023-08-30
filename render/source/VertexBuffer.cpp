//
//  VertexBuffer.cpp
//
//  Created by Grigory Shabaganov on 10.08.2013.
//

#include "gmt/render/VertexBuffer.h"

#include <cassert>

namespace gmt
{

namespace detail
{

GLuint createVertexBufferObject()
{
    GLuint vbo;
    glGenBuffers(1, &vbo);
    return vbo;
}

}

VertexBuffer::VertexBuffer(GLenum target)
    : target_{ target }
    , vbo_{ detail::createVertexBufferObject() }
{
}

VertexBuffer::VertexBuffer(VertexBuffer&& src) noexcept
    : target_{ src.target_ }
    , vbo_{ src.vbo_ }
    , size_{ src.size_ }
{
    src.vbo_ = 0;
    src.target_ = 0;
    src.size_ = 0;
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&src)
{
    if (vbo_) {
        glDeleteBuffers(1, &vbo_);
    }
    
    vbo_ = src.vbo_;
    target_ = src.target_;
    
    src.vbo_ = 0;
    src.target_ = 0;
    
    return *this;
}

VertexBuffer::~VertexBuffer()
{
    if (vbo_) {
        glDeleteBuffers(1, &vbo_);
    }    
}

VertexBufferEditor &VertexBufferEditor::commit(bool unbind)
{
    if (data_.empty()) {
        return *this;
    }

    target_->bind();
    
    for (auto &data : data_) {
        if (data.subdata) {
            assert(data.size <= target_->size_);
            glBufferSubData(target_->target_, data.offset, data.size, data.data);
        } else {
            glBufferData(target_->target_, data.size, data.data, data.usage);
            target_->size_ = data.size;
        }
    }
    data_.clear();
    
    if (unbind) {
        target_->unbind();
    }
    
    return *this;
}

VertexBufferEditor &VertexBufferEditor::clear()
{
    data_.clear();
    return *this;
}

}
