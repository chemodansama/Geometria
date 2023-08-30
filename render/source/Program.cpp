#include "gmt/render/Program.h"

#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>

#include "gmt/render/OpenGL.h"
#include "gmt/Utils.h"

namespace gmt
{

namespace detail
{

void logShaderInfo(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (!length) {
        return;
    }

    std::vector<char> data(length);
    glGetShaderInfoLog(shader, length, nullptr, data.data());
    utils::log(data.data());
}

void logProgramInfo(GLuint program)
{
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (!length) {
        return;
    }

    std::vector<GLchar> data(length);
    glGetProgramInfoLog(program, length, nullptr, data.data());
    utils::log(data.data());
}

bool checkCompileStatus(GLuint shader)
{
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        utils::log("Error compiling shader");
        logShaderInfo(shader);
        return false;
    }
    return true;
}

UniformControlBlock::UniformControlBlock(int location)
    : location_{ location }
{
}


bool UniformControlBlock::bind(int uniformId)
{
    if (uniformId_ == uniformId) {
        return false;
    }
    
    uniformId_ = uniformId;
    return true;
}
  
void loadAttributeLocations(GLuint program, std::unordered_map<std::string, Attribute> *locations)
{
    int attributesCount;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributesCount);

    GLint maxLen = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen);

    std::vector<GLchar> name(maxLen);
    for (int i = 0; i < attributesCount; i++) {
        GLsizei len;
        GLint size;
        GLenum type;

        glGetActiveAttrib(program, i, maxLen,  &len, &size, &type, name.data());
        auto location = glGetAttribLocation(program, name.data());
        auto namestr = std::string(name.begin(), name.begin() + len);
        Attribute p{ static_cast<GLuint>(location), size, type };
        utils::log("Attribute(", namestr, ") located at ", p.location());
        locations->emplace(std::move(namestr), std::move(p));
    }
}

void loadUniformLocations(GLuint program, std::vector<UniformInfo> *uniforms)
{
    GLint uniformsCount;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformsCount);

    GLint maxLength = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    std::vector<GLchar> buffer(maxLength);
    for (int i = 0; i < uniformsCount; i++) {
        GLsizei len;
        GLint size;
        GLenum type;

        glGetActiveUniform(program, i, maxLength,  &len, &size, &type, buffer.data());
        auto location = glGetUniformLocation(program, buffer.data());
        auto name = std::string(buffer.begin(), buffer.begin() + len);
        utils::log("Uniform(", name, ") located at ", location);
        uniforms->emplace_back(std::move(name), location);
    }
}

GLuint compileProgram(GLuint vs, GLuint tcs, GLuint tes, GLuint fs)
{
    auto program = glCreateProgram();
    glAttachShader(program, vs);
    if (tcs) {
        glAttachShader(program, tcs);
    }

    if (tes) {
        glAttachShader(program, tes);
    }

    glAttachShader(program, fs);
    glLinkProgram (program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
        utils::log("Failed to link program");
        detail::logProgramInfo(program);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

}

UniformBase::UniformBase()
    : controlBlock_{ nullptr }
    , id_{ -1 }
{
}

UniformBase::UniformBase(detail::UniformControlBlock *controlBlock, int id)
    : controlBlock_{ controlBlock }
    , id_{ id }
{
}

UniformBase::UniformBase(UniformBase&& source)
    : stale_{ source.stale_ }
    , controlBlock_{ source.controlBlock_ }
    , id_{ source.id_ }
{
    source.controlBlock_ = nullptr;
    source.id_ = -1;
}

UniformBase &UniformBase::operator=(UniformBase&& source)
{
    controlBlock_ = source.controlBlock_;
    id_ = source.id_;
    stale_ = source.stale_;

    source.controlBlock_ = nullptr;
    source.id_ = -1;
    
    return *this;
}

void UniformBase::stale()
{
    stale_ = true;
}

void Shader::release()
{
    if (shader_) {
        glDeleteShader(shader_);
        shader_ = 0;
    }    
}

Program::Program(const std::string &defines, const std::string &vertexSource,
        const std::string &fragmentSource)
{
    std::array<const char *, 3> sources;
    sources[0] = "#version 410 core\n";
    sources[1] = defines.data();
    
    sources[2] = vertexSource.data();
    mVertexShader = Shader::create<GL_VERTEX_SHADER>(sources);
    
    sources[2] = fragmentSource.data();
    mFragmentShader = Shader::create<GL_FRAGMENT_SHADER>(sources);
    
    mProgram = detail::compileProgram(mVertexShader, 0, 0, mFragmentShader);
    
    detail::loadAttributeLocations(mProgram, &mAttributes);
    detail::loadUniformLocations(mProgram, &mUniforms);
}

Program::Program(const std::string &defines, const std::string &vertexSource, const std::string &tessControlSource,
        const std::string &tessEvalSource, const std::string &fragmentSource)
{
    std::array<const char *, 3> sources;
    sources[0] = "#version 410 core\n";
    sources[1] = defines.data();

    sources[2] = vertexSource.data();
    mVertexShader = Shader::create<GL_VERTEX_SHADER>(sources);
    
    sources[2] = tessControlSource.data();
    mTessControlShader = Shader::create<GL_TESS_CONTROL_SHADER>(sources);
    
    sources[2] = tessEvalSource.data();
    mTessEvalShader = Shader::create<GL_TESS_EVALUATION_SHADER>(sources);
    
    sources[2] = fragmentSource.data();
    mFragmentShader = Shader::create<GL_FRAGMENT_SHADER>(sources);
    
    mProgram = detail::compileProgram(mVertexShader, mTessControlShader, mTessEvalShader, mFragmentShader);

    detail::loadAttributeLocations(mProgram, &mAttributes);
    detail::loadUniformLocations(mProgram, &mUniforms);
}

Program::~Program()
{
    glDeleteProgram(mProgram);
}

void Program::bind() const
{
    if (!mProgram) {
        return;
    }
    glUseProgram(mProgram);
}

const Attribute *Program::attribute(const std::string &name) const
{
    auto it = mAttributes.find(name);
    if (it == mAttributes.end()) {
        utils::log("Unknown attribute: ", name);
        return nullptr;
    }
    
    return &it->second;
}

int Program::getUniformLocation(const std::string &name) const {
    for (auto it = mUniforms.begin(); it != mUniforms.end(); ++it) {
        if (it->name().compare(name) == 0) {
            return it->location();
        }
    }
    return -1;
}

detail::UniformControlBlock *Program::getControlBlock(int location)
{
    auto it = uniformControlBlocks_.find(location);
    if (it == uniformControlBlocks_.end()) {
        auto controlBlock = detail::UniformControlBlock{ location };
        auto emplaceResult = uniformControlBlocks_.emplace(location, controlBlock);
        return &emplaceResult.first->second;
    } else {
        return &it->second;
    }
}

UniformInfo::UniformInfo(const std::string &name, int location)
    : name_(name)
    , location_(location)
{
}

UniformInfo::UniformInfo(std::string&& name, int location)
    : name_(std::move(name))
    , location_(location)
{
}

Attribute::Attribute(GLuint location, GLint size, GLenum type)
    : location_{ location }
    , size_{ size }
    , type_{ type }
{
}



}
