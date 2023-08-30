#pragma once

#include <memory>
#include <functional>

#include "OpenGL.h"

namespace gmt
{

class Texture
{
public:
    Texture(void) { glGenTextures(1, &texture_); }
    Texture(Texture&& t) noexcept : texture_{ t.texture_ } { t.texture_ = 0; }
    Texture &operator=(Texture&& t);
    ~Texture();
    
    template <GLenum target>
    void bind() const;
    
    template <GLenum target>
    static void unbind();
    
private:
    friend class FramebufferEditor;

    GLuint texture_;

    operator GLuint() const { return texture_; }
    
    template <GLenum target>
    static void checkTarget();
};

class TextureEditor
{
public:
    TextureEditor() = default;
    TextureEditor &setFilter(GLint minFilter, GLint magFilter);
    TextureEditor &setWrap(GLint wrapS, GLint wrapT);
    TextureEditor &setData(GLint width, GLint height, const GLvoid *data, GLint level = 0,
                           GLenum internalFormat = GL_RGBA,
                           GLenum format = GL_RGBA,
                           GLenum type = GL_UNSIGNED_BYTE);

    bool commit(Texture *texture, bool initial) const;

private:
    GLint minFilter_{ GL_LINEAR };
    GLint magFilter_{ GL_LINEAR };
    mutable bool filterDirty_{ false };
    
    GLint wrapS_{ GL_CLAMP_TO_EDGE };
    GLint wrapT_{ GL_CLAMP_TO_EDGE };
    mutable bool wrapDirty_{ false };

    GLint level_{ 0 };
    GLenum internalFormat_{ GL_RGBA8 };
    GLenum format_{ GL_RGBA };
    GLenum type_{ GL_UNSIGNED_BYTE };
    GLint width_{ 1 };
    GLint height_{ 1 };
    const GLvoid *data_{ nullptr };
    mutable bool dataDirty_{ false };
};

template <GLenum target>
void Texture::bind() const
{
    checkTarget<target>();
    glBindTexture(target, texture_);
}

template <GLenum target>
void Texture::unbind()
{
    checkTarget<target>();
    glBindTexture(target, 0);
}

template <GLenum target>
void Texture::checkTarget()
{
    static_assert(target == GL_TEXTURE_2D || target == GL_TEXTURE_RECTANGLE, "wrong target");
};


}
