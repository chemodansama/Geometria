//
//  InputLayout.cpp
//
//  Created by Grigory Shabaganov on 30.07.2017.
//

#include <cassert>

#include "gmt/render/InputLayout.h"
#include "gmt/Utils.h"

namespace gmt
{

namespace
{

GLint getScalarTypeSize(GLenum type)
{
    switch (type) {
    case GL_UNSIGNED_BYTE:
        return 1;

    case GL_FLOAT:
        return 4;
        
    case GL_DOUBLE:
        return 8;

    default:
        assert(false);
        return 0;
    }
}

bool unpackType(GLenum type, GLint *size, GLenum *scalarType, GLuint *scalarCount)
{
    *scalarCount = 1;
    
    switch (type) {
    case GL_FLOAT:
        *size = 1;
        *scalarType = GL_FLOAT;
        break;
        
    case GL_FLOAT_VEC2:
        *size = 2;
        *scalarType = GL_FLOAT;
        break;
        
    case GL_FLOAT_VEC3:
        *size = 3;
        *scalarType = GL_FLOAT;
        break;
        
    case GL_FLOAT_VEC4:
        *size = 4;
        *scalarType = GL_FLOAT;
        break;
    
    case GL_DOUBLE:
        *size = 1;
        *scalarType = GL_DOUBLE;
        break;
        
    case GL_DOUBLE_VEC2:
        *size = 2;
        *scalarType = GL_DOUBLE;
        break;
        
    case GL_DOUBLE_VEC3:
        *size = 3;
        *scalarType = GL_DOUBLE;
        break;
        
    case GL_DOUBLE_VEC4:
        *size = 4;
        *scalarType = GL_DOUBLE;
        break;
        
    case GL_FLOAT_MAT4:
        *scalarCount = 4;
        *size = 4;
        *scalarType = GL_FLOAT;
        break;
            
    default:
        return false;
    };

    return true;
}

}

InputLayoutEditor &InputLayoutEditor::bind(const VertexBuffer *buffer)
{
    buffers_.push_back(buffer);
    events_.push(detail::InputLayoutEditorEvent::BindBuffer);
    return *this;
}

InputLayoutEditor &InputLayoutEditor::setAttribute(const std::string &name, bool normalized,
	GLsizei stride, void *ptr, int instanced, int componentsCount, ScalarType scalarType)
{
    auto attribute = program_->attribute(name);
    if (!attribute) {
        utils::logf("Attribute not found: {}", name);
        return *this;
    }
    
    GLint size;
    GLenum shaderScalarType;
    GLuint shaderScalarCount;
    if (!unpackType(attribute->type(), &size, &shaderScalarType, &shaderScalarCount)) {
        utils::logf("Failed to unpack attribute({}) type: {}", name, attribute->type());
        return *this;
    }
    
    if (componentsCount > 0) {
        size = componentsCount;
    }

    shaderScalarType =
        scalarType == ScalarType::Float ? GL_FLOAT :
        scalarType == ScalarType::UnsignedByte ? GL_UNSIGNED_BYTE : shaderScalarType;

    auto scalarSize = getScalarTypeSize(shaderScalarType);
    for (GLuint i = 0; i < shaderScalarCount; i++) {
        detail::AttributeBinding binding;
        binding.location = attribute->location() + i;
        binding.size = size;
        binding.type = shaderScalarType;
        binding.ptr = (void*)(reinterpret_cast<uintptr_t>(ptr) 
            + static_cast<uintptr_t>(i) * size * scalarSize);
        binding.stride = stride;
        binding.normalized = normalized;
        binding.instanced = instanced;
        
        attributes_.push(binding);
        events_.push(detail::InputLayoutEditorEvent::BindAttribute);
    }
    nextAttributePtr_ = reinterpret_cast<uint8_t*>(ptr) 
        + static_cast<uintptr_t>(shaderScalarCount) * size * scalarSize;
    return *this;
}

InputLayoutEditor &InputLayoutEditor::appendAttribute(const std::string &name, bool normalized,
	GLsizei stride, int instanced, int componentsCount, ScalarType scalarType)
{
    return setAttribute(name, normalized, stride, nextAttributePtr_, instanced, componentsCount,
        scalarType);
}

void InputLayoutEditor::commit(InputLayout *layout)
{
    layout->edit(*this);
}

void InputLayout::edit(InputLayoutEditor &builder)
{
    vao_.bind();

    size_t bufferIndex = 0;
    while (!builder.events_.empty()) {
        auto event = builder.events_.front();
        builder.events_.pop();
        
        if (event == detail::InputLayoutEditorEvent::BindBuffer) {
            builder.buffers_[bufferIndex]->bind();
            bufferIndex += 1;
        } else if (event == detail::InputLayoutEditorEvent::BindAttribute) {
            auto &attribute = builder.attributes_.front();
            glVertexAttribPointer(attribute.location,             // index
                                  attribute.size,                 // number of values per vertex
                                  attribute.type,                 // type (GL_FLOAT)
                                  attribute.normalized ? GL_TRUE : GL_FALSE,
                                  attribute.stride,               // stride (offset to next vertex data)
                                  (const GLvoid*)attribute.ptr);
            glEnableVertexAttribArray(attribute.location);
            
            if (attribute.instanced != 0) {
                glVertexAttribDivisor(attribute.location, attribute.instanced);
            }
            
            builder.attributes_.pop();
        } else {
            assert(false);
        }
    }

    vao_.unbind();
    
    for (auto &buffer : builder.buffers_) {
        buffer->unbind();
    }

    auto err = glGetError();
    assert(err == GL_NO_ERROR);
}

}
