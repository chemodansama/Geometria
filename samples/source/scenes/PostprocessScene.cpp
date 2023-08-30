//
//  PostprocessScene.cpp
//
//  Created by Grigory Shabaganov on 27.07.2017.
//

#include "PostprocessScene.h"

#include <iostream>
#include <utility>
#include <array>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Common.h"
#include "ShaderTypes.h"

namespace // internal
{

gmt::Renderbuffer createDepthRenderbuffer(int w, int h)
{
    gmt::Renderbuffer rb;
    rb.setStorage(gmt::Renderbuffer::Format::DEPTH_24, w, h);
    return rb;
}

gmt::Texture createRenderTexture(int w, int h)
{
    gmt::Texture texture;
    auto success = gmt::TextureEditor{}
        .setData(w, h, nullptr)
        .commit(&texture, true);
    assert(success);
    return texture;
}

gmt::Framebuffer createFramebuffer(const gmt::Texture &renderTexture,
    const gmt::Renderbuffer &depthBuffer)
{
    gmt::Framebuffer framebuffer;
    auto success = gmt::FramebufferEditor{}
            .addRenderTexture<GL_COLOR_ATTACHMENT0>(renderTexture)
            .addRenderBuffer<GL_DEPTH_ATTACHMENT>(depthBuffer)
            .commit(&framebuffer);
    assert(success);
    return framebuffer;
}

} // internal namespace

PostprocessScene::PostprocessScene(gmt::Context *context)
    : gmt::EnableWeakFromThis<gmt::ContextListener>{ this }
    , gmt::EnableWeakFromThis<gmt::FrustumListener>{ this }
    , context_{ context }
    , program_{
        std::string{},
        gmt::assets::load<std::string>(R"(Assets/Shaders/colored.vert)"),
        gmt::assets::load<std::string>(R"(Assets/Shaders/colored.frag)") }
    , postprocessProgram_{
        std::string{},
        gmt::assets::load<std::string>(R"(Assets/Shaders/colored.vert)"),
        gmt::assets::load<std::string>(R"(Assets/Shaders/colored.frag)") }
    , viewProjUniform_{ program_.uniform<gmt::UniformMat4f>("u_ModelviewProjectionMat") }
    , postprocessViewProjUniform_{ postprocessProgram_.uniform<gmt::UniformMat4f>("u_ModelviewProjectionMat") }
    , textureUniform_{ program_.uniform<gmt::Uniform1i>("u_Texture") }
    , postprocessTextureUniform_{ postprocessProgram_.uniform<gmt::Uniform1i>("u_Texture") }
    , layout_{ mesh_.createInputLayout(program_) }
    , postprocessLayout_{ mesh_.createInputLayout(postprocessProgram_) }
    , texture_{ loadCheckerTexture() }
    , renderTexture_{ createRenderTexture(context->width(), context->height()) }
    , depthBuffer_{ createDepthRenderbuffer(context->width(), context->height()) }
    , framebuffer_{ createFramebuffer(renderTexture_, depthBuffer_) }
{
    assert(postprocessProgram_.valid());

    context->addObserver(this);
    frustum_.addObserver(this);
    frustum_.viewSetIdentity();
    frustum_.viewTranslate(0.0f, 0.0f, -5.0f);
    onResize(context->width(), context->height());
    viewProjUniform_.set(glm::value_ptr(cubeMat_), false);

    postprocessFrustum_.viewSetIdentity();
    postprocessFrustum_.projSetOrtho(0, 1, 0, 1, -1, 1);
    postprocessViewProjUniform_.set(postprocessFrustum_.getViewProjPtr(), false);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(PrimitiveRestartIndex<GLushort>::value);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    updateTime_ = std::chrono::high_resolution_clock::now();
}

void PostprocessScene::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> dt = now - updateTime_;
    cubeAngle_ += static_cast<float>(dt.count() * glm::pi<float>());

    auto rotAxis = glm::normalize(glm::vec3{ 1.0f, 1.0f, 1.0f });
    glm::quat cubeQuat = glm::angleAxis(cubeAngle_, rotAxis);
    
    cubeMat_ = frustum_.getViewProj() * glm::mat4_cast(cubeQuat);
    viewProjUniform_.stale();
    updateTime_ = now;
}

void PostprocessScene::render()
{
    frustum_.update();

//============================================================================================================

    framebuffer_.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    program_.bind();
    layout_.bind();
    viewProjUniform_.apply();
    
    glActiveTexture(GL_TEXTURE0);
    texture_.bind<GL_TEXTURE_2D>();
    textureUniform_.apply();

    mesh_.drawCube();

    framebuffer_.unbind();

//============================================================================================================

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    postprocessProgram_.bind();
    postprocessLayout_.bind();
    postprocessViewProjUniform_.apply();
    glActiveTexture(GL_TEXTURE0);
    renderTexture_.bind<GL_TEXTURE_2D>();
    postprocessTextureUniform_.apply();

    mesh_.drawQuad();

    // optionally unbind layout and program:
//    postprocessLayout_.unbind();
//    postprocessProgram_.unbind();
    
//============================================================================================================
    
}

void PostprocessScene::onResize(int width, int height)
{
    auto k = static_cast<float>(width) / height;
    frustum_.projSetPerspective(45.0f, k, 0.1f, 10.0f);
    glViewport(0, 0, width, height);

    auto success = gmt::TextureEditor{}
        .setData(context_->width(), context_->height(), nullptr)
        .commit(&renderTexture_, false);
    assert(success);
    depthBuffer_.setStorage(gmt::Renderbuffer::Format::DEPTH_24,
        context_->width(), context_->height());
}

void PostprocessScene::onViewProjChanged()
{
}
