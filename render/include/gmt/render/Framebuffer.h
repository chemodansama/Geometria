#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <utility>

#include "OpenGL.h"
#include "Texture.h"

namespace gmt
{

class Renderbuffer
{
public:
    enum class Format : uint8_t
    {
        DEPTH_16, DEPTH_24, DEPTH_32,
        DEPTH_24_STENCIL_8,
        LAST_FORMAT
    };

    Renderbuffer(void) { glGenRenderbuffers(1, &renderbuffer_); }
    Renderbuffer(Renderbuffer&& f) noexcept : renderbuffer_{ f.renderbuffer_ } { f.renderbuffer_ = 0; }
    Renderbuffer &operator=(Renderbuffer&& f);
    ~Renderbuffer();

    Renderbuffer &setStorage(Format format, int width, int height);

private:
    friend class FramebufferEditor;
    
    GLuint renderbuffer_;
    
    operator GLuint() const { return renderbuffer_; }
};

class Framebuffer
{
public:
    Framebuffer(void) { glGenFramebuffers(1, &framebuffer_); }
    Framebuffer(Framebuffer&& f) noexcept: framebuffer_{ f.framebuffer_ } { f.framebuffer_ = 0; }
    Framebuffer &operator=(Framebuffer&& f);
    ~Framebuffer();
 
    void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_); }
    static void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

private:
    GLuint framebuffer_;
};

class FramebufferEditor
{
public:
    template <GLenum target>
    FramebufferEditor &addRenderBuffer(const Renderbuffer &renderBuffer);

    template <GLenum target>
    FramebufferEditor &addRenderTexture(const Texture &renderTexture);

    bool commit(Framebuffer *framebuffer);

private:
    std::vector<std::pair<GLenum, GLuint>> renderBuffers_;
    std::vector<std::pair<GLenum, GLuint>> renderTextures_;
};

template <GLenum target>
FramebufferEditor &FramebufferEditor::addRenderBuffer(const Renderbuffer &renderBuffer)
{
    renderBuffers_.emplace_back(target, static_cast<GLuint>(renderBuffer));
    return *this;
}

template <GLenum target>
FramebufferEditor &FramebufferEditor::addRenderTexture(const Texture &renderTexture)
{
    renderTextures_.emplace_back(target, static_cast<GLuint>(renderTexture));
    return *this;
}

}
