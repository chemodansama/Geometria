//  
//  VertexBuffer.h
//  
//  Created by Grigory Shabaganov on 10.08.2013.
//

#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <type_traits>
#include <vector>

#include "gmt/Render/OpenGL.h"

namespace gmt
{

namespace detail
{

constexpr bool isValidVertexBufferTarget(GLenum target)
{
    return target == GL_ARRAY_BUFFER
        || target == GL_COPY_READ_BUFFER
        || target == GL_COPY_WRITE_BUFFER
        || target == GL_ELEMENT_ARRAY_BUFFER
        || target == GL_PIXEL_PACK_BUFFER
        || target == GL_PIXEL_UNPACK_BUFFER
        || target == GL_TEXTURE_BUFFER
        || target == GL_TRANSFORM_FEEDBACK_BUFFER
        || target == GL_UNIFORM_BUFFER;
};

}

class VertexBuffer
{
public:
    template <typename T>
    struct Internal { using Type = int; };

    template <GLenum target, typename V = float>
    static VertexBuffer create(const V *data = nullptr, size_t count = 0u, 
        GLenum usage = GL_STATIC_DRAW);
    
    template <GLenum target, typename Container>
    static VertexBuffer create(const Container &data, GLenum usage = GL_STATIC_DRAW,
                               typename Internal<typename Container::value_type>::Type = 0);
    
    VertexBuffer(VertexBuffer&) = delete;
    VertexBuffer &operator=(VertexBuffer&) = delete;
    
    VertexBuffer(VertexBuffer&& src) noexcept;
    VertexBuffer &operator=(VertexBuffer &&src);
    
    ~VertexBuffer();
    
    void bind() const { glBindBuffer(target_, vbo_); }
    void unbind() const { glBindBuffer(target_, 0); }
    
private:
    friend class VertexBufferEditor;

    GLenum target_;
    GLuint vbo_;
    size_t size_{ 0 };

    VertexBuffer(GLenum target);
};

class VertexBufferEditor
{
public:
    VertexBufferEditor(VertexBuffer *target) : target_{ target }
    {
    }

    template <typename T>
    struct Internal { using Type = int; };

    template <typename V>
    VertexBufferEditor &bufferData(const V *data, size_t count, GLenum usage = GL_STATIC_DRAW);
    
    template <typename Container>
    VertexBufferEditor &bufferData(const Container &data, GLenum usage = GL_STATIC_DRAW,
                                   typename Internal<typename Container::value_type>::Type = 0);
    
    template <typename V>
    VertexBufferEditor &bufferSubData(size_t offset, const V *data, size_t count);
    
    template <typename Container>
    VertexBufferEditor &bufferSubData(size_t offset, const Container &data,
                                      typename Internal<typename Container::value_type>::Type = 0);
    
    VertexBufferEditor &commit(bool unbind = true);
    VertexBufferEditor &clear();

private:
    struct VertexBufferData
    {
        bool subdata;
        size_t offset;
        size_t size;
        const void *data;
        GLenum usage;
    };

    VertexBuffer *target_;
    std::vector<VertexBufferData> data_;
};

template <typename Container>
VertexBufferEditor &VertexBufferEditor::bufferData(const Container &data, GLenum usage,
                                                   typename Internal<typename Container::value_type>::Type)
{
    auto size = data.size() * sizeof(typename Container::value_type);
    data_.push_back(VertexBufferData{ false, 0, size, data.data(), usage });
    return *this;
}

template <typename V>
VertexBufferEditor &VertexBufferEditor::bufferData(const V *data, size_t count, GLenum usage)
{
    data_.push_back(VertexBufferData{ false, 0, count * sizeof(V), data, usage });
    return *this;
}

template <typename V>
VertexBufferEditor &VertexBufferEditor::bufferSubData(size_t offset, const V *data, size_t count)
{
    data_.push_back(VertexBufferData{ true, offset, count * sizeof(V), data, 0 });
    return *this;
}

template <typename Container>
VertexBufferEditor &VertexBufferEditor::bufferSubData(size_t offset, const Container &data,
                                                      typename Internal<typename Container::value_type>::Type)
{
    auto size = data.size() * sizeof(typename Container::value_type);
    data_.push_back(VertexBufferData{ true, offset, size, data.data(), 0 });
    return *this;
}

template <GLenum target, typename V>
VertexBuffer VertexBuffer::create(const V *data, size_t count, GLenum usage)
{
    static_assert(detail::isValidVertexBufferTarget(target), "Wrong vertex buffer target");
    
    VertexBuffer v{ target };
    VertexBufferEditor{ &v }
        .bufferData(data, count, usage)
        .commit(false);
    return v;
}

template <GLenum target, typename Container>
VertexBuffer VertexBuffer::create(const Container &data, GLenum usage,
                                  typename Internal<typename Container::value_type>::Type)
{
    static_assert(detail::isValidVertexBufferTarget(target), "Wrong vertex buffer target");
    
    VertexBuffer v{ target };
    VertexBufferEditor{ &v }
        .bufferData(data, usage)
        .commit(false);
    return v;
}

} /* namespace falcon */
