#include "gmt/render/Framebuffer.h"

#include <cassert>
#include <array>

#include "gmt/Debug.h"
#include "gmt/Utils.h"

namespace gmt
{

namespace
{

bool isFramebufferOk(GLenum err)
{
    switch (err) {
    case GL_FRAMEBUFFER_COMPLETE:
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        utils::log("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        utils::log("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        utils::log("GL_FRAMEBUFFER_UNSUPPORTED");
        break;
        
    default:
        utils::logf("Unknown framebuffer error: {}", err);
    }
    return false;
}

}

Renderbuffer &Renderbuffer::operator=(Renderbuffer&& f)
{
    if (renderbuffer_) {
        glDeleteRenderbuffers(1, &renderbuffer_);
    }
    renderbuffer_ = f.renderbuffer_;
    f.renderbuffer_ = 0;
    return *this;
};

Renderbuffer::~Renderbuffer()
{
    if (renderbuffer_) {
        glDeleteRenderbuffers(1, &renderbuffer_);
    }
}

Renderbuffer &Renderbuffer::setStorage(Format format, int width, int height)
{
    GLenum formatMapping[]
    {
        GL_DEPTH_COMPONENT16,
        GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT32,
        GL_DEPTH24_STENCIL8,
    };
    constexpr auto formatsCount = static_cast<size_t>(Format::LAST_FORMAT);
    static_assert(std::extent<decltype(formatMapping)>::value == formatsCount, "");

    const auto formatIndex = static_cast<int>(format);

    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, formatMapping[formatIndex], width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return *this;
}

Framebuffer &Framebuffer::operator=(Framebuffer&& f)
{
    if (framebuffer_) {
        glDeleteFramebuffers(1, &framebuffer_);
    }

    framebuffer_ = f.framebuffer_;
    f.framebuffer_ = 0;
    return *this;
}

Framebuffer::~Framebuffer()
{
    if (framebuffer_) {
        glDeleteFramebuffers(1, &framebuffer_);
    }
}

bool FramebufferEditor::commit(Framebuffer *framebuffer)
{
    assert(framebuffer);
    framebuffer->bind();
    
    for (const auto &texture : renderTextures_) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, texture.first, GL_TEXTURE_2D, texture.second, 0);
    }
    
    for (const auto &buffer : renderBuffers_) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, buffer.first, GL_RENDERBUFFER, buffer.second);
    }
    
    auto framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    Framebuffer::unbind();
    
    if (!isFramebufferOk(framebufferStatus)) {
        debug::printStacktrace();
        return false;
    }
    return true;
}

}
