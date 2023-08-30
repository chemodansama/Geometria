//
//  InstancingScene.cpp
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#include "InstancingScene.h"

#include <iostream>
#include <utility>
#include <array>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Common.h"
#include "ShaderTypes.h"

InstancingScene::InstancingScene(gmt::Context *context)
    : gmt::EnableWeakFromThis<gmt::ContextListener>{ this }
    , gmt::EnableWeakFromThis<gmt::FrustumListener>{ this }
    , context_{ context }
    , program_{
        std::string{},
        gmt::assets::load<std::string>(R"(assets/shaders/instanced.vert)"),
        gmt::assets::load<std::string>(R"(assets/shaders/instanced.frag)") }
    , viewProjUniform_{ program_.uniform<gmt::UniformMat4f>("u_ViewProjectionMat") }
    , textureUniform_{ program_.uniform<gmt::Uniform1i>("u_Texture") }
    , mesh_{ true }
    , layout_{ mesh_.createInputLayout(program_) }
    , texture_{ loadCheckerTexture() }
{
    context->addObserver(this);
    frustum_.addObserver(this);
    frustum_.viewSetIdentity();
    frustum_.viewTranslate(0.0f, 0.0f, -5.0f);
    onResize(context->width(), context->height());
    viewProjUniform_.set(frustum_.getViewProjPtr(), false);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(PrimitiveRestartIndex<GLushort>::value);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void InstancingScene::update()
{
    mesh_.updateInstances();
}

void InstancingScene::render()
{
    frustum_.update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    program_.bind();
    layout_.bind();
    viewProjUniform_.apply();

    glActiveTexture(GL_TEXTURE0);
    texture_.bind<GL_TEXTURE_2D>();
    textureUniform_.apply();

    mesh_.drawCube();
}

void InstancingScene::onResize(int width, int height)
{
    const auto aspect = static_cast<float>(width) / height;
    frustum_.projSetPerspective(45.0f, aspect, 0.1f, 10.0f);
    glViewport(0, 0, width, height);
}

void InstancingScene::onViewProjChanged()
{
    viewProjUniform_.stale();
}
