#pragma once

#include <algorithm>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include "gmt/render/OpenGL.h"

namespace gmt
{

namespace detail
{

bool checkCompileStatus(GLuint shader);

template <typename It>
void compileShader(GLuint shader, It sourceBegin, It sourceEnd);
    
class UniformControlBlock
{
public:
    UniformControlBlock(int location);
    
    int location() const { return location_; }
    
    // Returns true if program's uniform at control block's location is different
    // from the uniform defined by passed uniformId.
    bool bind(int uniformId);
    
private:
    const int location_;
    int uniformId_{ -1 };
};

}

class UniformBase
{
public:
    UniformBase();
    UniformBase(detail::UniformControlBlock *controlBlock, int id);

    UniformBase(UniformBase&& source);
    UniformBase &operator=(UniformBase&& source);

    void stale();

protected:
    detail::UniformControlBlock *controlBlock() const { return controlBlock_; }
    int id() const { return id_; }
    mutable bool stale_{ true };

private:
    detail::UniformControlBlock *controlBlock_;
    int id_;
};

template <typename T>
class Uniform : public UniformBase
{
public:
    using UniformBase::UniformBase;
    
    void set(T value);
    void apply() const;
    
private:
    T value_{};
    virtual void bind(int location, T value) const = 0;
};

template <typename T>
class UniformMat : public UniformBase
{
public:
    using UniformBase::UniformBase;
    
    void set(const T *value, bool transpose);
    void apply() const;
    
private:
    const T *value_{};
    bool transpose_{ false };
    
    virtual void bind(int location, const T *value, bool transpose) const = 0;
};

class UniformInfo
{
public:
    UniformInfo(const std::string &name, int location);
    UniformInfo(std::string&& name, int location);

    const std::string &name() const  { return name_; }
    int location() const  { return location_; }

private:
    std::string name_;
    int location_;
};

class Attribute
{
public:
    Attribute(GLuint location, GLint size, GLenum type);
    GLuint location() const  { return location_; }
    GLint size() const  { return size_; }
    GLenum type() const  { return type_; }

private:
    const GLuint location_;
    const GLint size_;
    const GLenum type_;
};

class Shader
{
public:
    template <GLuint type, typename T>
    static Shader create(const T &sources);

    template <GLuint type, typename It>
    static Shader create(It sourceBegin, It sourceEnd);

    Shader() : shader_(0) {}
    Shader(Shader&& s) noexcept : shader_(s.shader_) { s.shader_ = 0; }
    Shader &operator=(Shader&& s) { release(); shader_ = s.shader_; s.shader_ = 0; return *this; }
    ~Shader() { release(); }

    operator GLuint() const { return shader_; }

private:
    Shader(GLuint shader) : shader_(shader) {}
    GLuint shader_;

    void release();
};

class Program
{
public:
    Program(const std::string &defines, const std::string &vertexSource, const std::string &fragmentSource);
    Program(const std::string &defines, 
            const std::string &vertexSource, const std::string &tessControlSource,
            const std::string &tessEvalSource, const std::string &fragmentSource);
    Program(const Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(const Program&) = delete;
    Program& operator=(Program&&) = delete;
    ~Program();
    
    bool valid() const { return mProgram != 0; }
    
    void bind() const;
    static void unbind() { glUseProgram(0); }
    
    const Attribute *attribute(const std::string &name) const;
    
    template <typename T>
    T uniform(const std::string &name);
    
private:
    Shader mVertexShader;
    Shader mTessControlShader;
    Shader mTessEvalShader;
    Shader mFragmentShader;
    GLuint mProgram;

    std::vector<UniformInfo> mUniforms;
    std::unordered_map<std::string, Attribute> mAttributes;
    std::unordered_map<int, detail::UniformControlBlock> uniformControlBlocks_;
    int nextUniformId_{ 0 };
    
    detail::UniformControlBlock *getControlBlock(int location);
    int getUniformLocation(const std::string &name) const;
};

class Uniform1i : public Uniform<int>
{
public:
    using Uniform<int>::Uniform;

private:
    void bind(int location, int value) const override
    {
        glUniform1i(location, value);
    }
};

class Uniform1f : public Uniform<float>
{
public:
    using Uniform<float>::Uniform;

private:
    void bind(int location, float value) const override
    {
        glUniform1f(location, value);
    }
};

class Uniform4f : public Uniform<float*>
{
public:
    using Uniform<float*>::Uniform;

private:
    void bind(int location, float *value) const override
    {
        glUniform4fv(location, 1, value);
    }
};

class UniformMat4f : public UniformMat<float>
{
public:
    using UniformMat<float>::UniformMat;

private:
    void bind(int location, const float *value, bool transpose) const override
    {
        glUniformMatrix4fv(location, 1, transpose, value);
    }
};

class UniformMat3f : public UniformMat<float>
{
public:
    using UniformMat<float>::UniformMat;

private:
    void bind(int location, const float *value, bool transpose) const override
    {
        glUniformMatrix3fv(location, 1, transpose, value);
    }
};

// Templates implementation ==================================================================================

template <typename T>
T Program::uniform(const std::string &name)
{
    auto location = getUniformLocation(name);
    auto uniformId = nextUniformId_;
    nextUniformId_ += 1;
    return T{ getControlBlock(location), uniformId };
}

template <GLuint type, typename T>
Shader Shader::create(const T &sources)
{
    return create<type>(sources.begin(), sources.end());
}

template <GLuint type, typename It>
Shader Shader::create(It sourceBegin, It sourceEnd)
{
    static_assert(type == GL_VERTEX_SHADER
                  || type == GL_FRAGMENT_SHADER
                  || type == GL_TESS_CONTROL_SHADER
                  || type == GL_TESS_EVALUATION_SHADER,
                  "wrong type");
    
    auto shader = glCreateShader(type);
    detail::compileShader(shader, sourceBegin, sourceEnd);
    if (!detail::checkCompileStatus(shader)) {
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

template <typename T>
void Uniform<T>::set(T value)
{
    assert(controlBlock());
    value_ = value;
    stale_ = true;
}

template <typename T>
void Uniform<T>::apply() const
{
    assert(controlBlock());
    if (controlBlock()->bind(id()) || stale_) {
        bind(controlBlock()->location(), value_);
        stale_ = false;
    }
}

template <typename T>
void UniformMat<T>::set(const T *value, bool transpose)
{
    assert(controlBlock());
    value_ = value;
    transpose_ = transpose;
    stale_ = true;
}

template <typename T>
void UniformMat<T>::apply() const
{
    assert(controlBlock());
    if (controlBlock()->bind(id()) || stale_) {
        bind(controlBlock()->location(), value_, transpose_);
        stale_ = false;
    }
}

namespace detail
{

template <typename It>
void compileShader(GLuint shader, It sourceBegin, It sourceEnd)
{
    auto size = std::distance(sourceBegin, sourceEnd);
    
    std::vector<GLint> lengths;
    lengths.reserve(size);
    std::transform(sourceBegin, sourceEnd, std::back_inserter(lengths),
                   [](const char *s){ return (GLint)strlen(s); });
    
    std::vector<const char*> csource;
    csource.reserve(size);
    std::copy(sourceBegin, sourceEnd, std::back_inserter(csource));
    
    glShaderSource(shader, static_cast<GLsizei>(size), csource.data(), lengths.data());
    glCompileShader(shader);
}

}

}
