#include "gmt/render/Texture.h"
#include "gmt/Debug.h"

namespace gmt
{

Texture::~Texture()
{
    if (texture_) {
        glDeleteTextures(1, &texture_);
    }
}

Texture &Texture::operator=(Texture&& t)
{
    if (texture_) {
        glDeleteTextures(1, &texture_);
    }
    texture_ = t.texture_;
    t.texture_ = 0;
    return *this;
}

TextureEditor &TextureEditor::setFilter(GLint minFilter, GLint magFilter)
{
    minFilter_ = minFilter;
    magFilter_ = magFilter;
    filterDirty_ = true;
    return *this;
}

TextureEditor &TextureEditor::setWrap(GLint wrapS, GLint wrapT)
{
    wrapS_ = wrapS;
    wrapT_ = wrapT;
    wrapDirty_ = true;
    return *this;
}

TextureEditor &TextureEditor::setData(GLint width, GLint height, const GLvoid *data, GLint level,
                                      GLenum internalFormat,
                                      GLenum format,
                                      GLenum type)
{
    width_ = width;
    height_ = height;
    data_ = data;
    
    level_ = level;
    internalFormat_ = internalFormat;
    format_ = format;
    type_ = type;

    dataDirty_ = true;
    return *this;
}

bool TextureEditor::commit(Texture *texture, bool initial) const
{
    texture->bind<GL_TEXTURE_2D>();
    
    if (filterDirty_ || initial) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_);
        filterDirty_ = false;
    }
    
    if (wrapDirty_ || initial) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT_);
        wrapDirty_ = false;
    }
    
    if (dataDirty_ || initial) {
        glTexImage2D(GL_TEXTURE_2D, level_, internalFormat_, width_, height_, 0, format_, type_, data_);
        dataDirty_ = false;
    }
    
    Texture::unbind<GL_TEXTURE_2D>();
    
    if (auto err = glGetError()) {
        gmt::debug::printStacktrace();
        return false;
    }
    
    return true;
}

}
