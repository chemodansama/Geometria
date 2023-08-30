//
//  SampleMesh.cpp
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#include "SampleMesh.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Common.h"
#include "ShaderTypes.h"

namespace
{

gmt::VertexBuffer createVertexBuffer()
{
    Vertex vertices[] {
        // fullscreen quad
        { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        
        // front
        { { -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
        { {  1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
        
        // back
        { {  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
        { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
        { { -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
        { {  1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
        
        // left
        { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        
        // right
        { {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        
        // bottom
        { { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        
        // top
        { { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
    };
    
    constexpr auto verticesCount = std::extent<decltype(vertices)>::value;
    static_assert(verticesCount == 7 * 4, "");
    
    auto buffer = gmt::VertexBuffer::create<GL_ARRAY_BUFFER>((Vertex*)nullptr, verticesCount);
    
    gmt::VertexBufferEditor{ &buffer }
            .bufferSubData(0, vertices, verticesCount)
            .commit();
    
    return buffer;
}

gmt::VertexBuffer createIndicesBuffer()
{
    std::array<GLushort, 7 * 4 + 5> indicesData {
        // fullscreen box
        0, 1, 2, 3,
        4, 5, 6, 7, PrimitiveRestartIndex<GLushort>::value,
        8, 9, 10, 11, PrimitiveRestartIndex<GLushort>::value,
        12, 13, 14, 15, PrimitiveRestartIndex<GLushort>::value,
        16, 17, 18, 19, PrimitiveRestartIndex<GLushort>::value,
        20, 21, 22, 23, PrimitiveRestartIndex<GLushort>::value,
        24, 25, 26, 27
    };
    return gmt::VertexBuffer::create<GL_ELEMENT_ARRAY_BUFFER>(indicesData);
}

}

SampleMesh::SampleMesh(bool instanced)
    : vertices_{ createVertexBuffer() }
    , indices_{ createIndicesBuffer() }
{
    if (instanced) {
        for (int i = 0; i < 4; i++) {
            instancesData_[i] = glm::mat4x4{};
        }
        instances_.reset(new gmt::VertexBuffer{ gmt::VertexBuffer::create<GL_ARRAY_BUFFER>(instancesData_, GL_DYNAMIC_DRAW) });
    }
    
    updateTime_ = std::chrono::high_resolution_clock::now();
}

gmt::InputLayout SampleMesh::createInputLayout(const gmt::Program &program) const
{
    gmt::InputLayout inputlayout;
    gmt::InputLayoutEditor editor{ &program };
    
    editor
        .bind(&vertices_)
        .bind(&indices_)
        .appendAttribute("in_position", false, sizeof(Vertex))
        .appendAttribute("in_color", false, sizeof(Vertex))
        .appendAttribute("in_texcoords", false, sizeof(Vertex));
    
    if (instances_) {
        editor
            .bind(instances_.get())
            .setAttribute("in_ModelMat", false, sizeof(glm::mat4x4), nullptr, 1);
    }
    
    editor.commit(&inputlayout);
    return inputlayout;
}

void SampleMesh::updateInstances()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dt = now - updateTime_;
    updateTime_ = now;

    cubeAngle_ += static_cast<float>(dt.count() * glm::pi<float>());

    auto rotAxis = glm::normalize(glm::vec3{ 1.0f, 1.0f, 1.0f });
    glm::quat cubeQuat = glm::angleAxis(cubeAngle_, rotAxis);
    
    if (instances_) {
        for (int i = 0; i < (int)instancesData_.size(); i++) {
            instancesData_[i] = glm::mat4x4{};
            instancesData_[i] = glm::translate(instancesData_[i], glm::vec3{ i / 2 * 2 - 1, i % 2 * 2 - 1, 0.0f });
            instancesData_[i] = glm::scale(instancesData_[i], glm::vec3{ 0.25f, 0.25f, 0.25f });
            instancesData_[i] = instancesData_[i] * glm::mat4_cast(cubeQuat);
        }

        gmt::VertexBufferEditor{ instances_.get() }
            .bufferSubData(0, instancesData_)
            .commit();
    }
}

void SampleMesh::drawCube() const
{
    GLsizei triPerInstance = 6 * 4 + 5;
    GLvoid *cubeOffset = (void*)(4 * sizeof(GLushort));
    if (instances_) {
        glDrawElementsInstanced(GL_TRIANGLE_FAN, triPerInstance, GL_UNSIGNED_SHORT, cubeOffset, (GLsizei)instancesData_.size());
    } else {
        glDrawElements(GL_TRIANGLE_FAN, triPerInstance, GL_UNSIGNED_SHORT, cubeOffset);
    }
}

void SampleMesh::drawQuad() const
{
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, nullptr);
}
