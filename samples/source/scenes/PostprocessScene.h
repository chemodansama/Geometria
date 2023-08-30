//
//  PostprocessScene.h
//
//  Created by Grigory Shabaganov on 27.07.2017.
//

#pragma once

#include <chrono>
#include <memory>

#include "gmt/gmt.h"
#include "gmt/render/Context.h"
#include "gmt/render/Frustum.h"
#include "gmt/render/Texture.h"
#include "gmt/render/Framebuffer.h"
#include "gmt/GeometriaEx.h"
#include "SampleMesh.h"

class PostprocessScene
    : private gmt::ContextListener
    , private gmt::FrustumListener
    , private gmt::EnableWeakFromThis<gmt::ContextListener>
    , private gmt::EnableWeakFromThis<gmt::FrustumListener>
{
public:
    PostprocessScene(gmt::Context *context);

    void update();
    void render();

private:
    void onResize(int width, int height) override;
    void onViewProjChanged() override;

    gmt::Context *context_;
    gmt::Program program_;
    gmt::Program postprocessProgram_;
    gmt::UniformMat4f viewProjUniform_;
    gmt::UniformMat4f postprocessViewProjUniform_;
    gmt::Uniform1i textureUniform_;
    gmt::Uniform1i postprocessTextureUniform_;
    
    gmt::Frustum frustum_;
    gmt::Frustum postprocessFrustum_;

    SampleMesh mesh_;

    gmt::InputLayout layout_;
    gmt::InputLayout postprocessLayout_;
    
    gmt::Texture texture_;
    
    gmt::Texture renderTexture_;
    gmt::Renderbuffer depthBuffer_;
    gmt::Framebuffer framebuffer_;
    
    float cubeAngle_{ 0.0f };
    glm::mat4x4 cubeMat_;
    std::chrono::high_resolution_clock::time_point updateTime_;
};
