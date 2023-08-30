//
//  InputLayout.h
//
//  Created by Grigory Shabaganov on 30.07.2017.
//

#pragma once

#include <vector>
#include <queue>

#include "OpenGL.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Program.h"

namespace gmt
{

namespace detail
{

struct AttributeBinding
{
    GLuint location;
    GLint size;
    GLenum type;
    bool normalized;
    GLsizei stride;
    void *ptr;
    
    int instanced;
};

enum class InputLayoutEditorEvent
{
    BindAttribute,
    BindBuffer,
};

}

class InputLayout;

class InputLayoutEditor
{
public:
    InputLayoutEditor(const Program *program) : program_{ program }
    {
    }

    InputLayoutEditor &bind(const VertexBuffer *buffer);
    InputLayoutEditor &setAttribute(const std::string &name, bool normalized, GLsizei stride, void *ptr, int instanced = 0);
    InputLayoutEditor &appendAttribute(const std::string &name, bool normalized, GLsizei stride, int instanced = 0);

    void commit(InputLayout *layout);

private:
    friend class InputLayout;
    
    std::queue<detail::InputLayoutEditorEvent> events_;
    
    std::vector<const VertexBuffer*> buffers_;
    const Program *program_;
    
    std::queue<detail::AttributeBinding> attributes_;
    void *nextAttributePtr_{ nullptr };
};
    
class InputLayout
{
public:
    InputLayout() = default;
    void bind() const { vao_.bind(); }
    static void unbind() { VertexArray::unbind(); }
    
private:
    friend class InputLayoutEditor;
    
    VertexArray vao_;
    
    void edit(InputLayoutEditor &builder);
};
    
}
