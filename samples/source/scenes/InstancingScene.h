//
//  InstancingScene.h
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#pragma once

#include "gmt/gmt.h"
#include "gmt/render/Context.h"
#include "gmt/render/Frustum.h"
#include "gmt/render/Texture.h"
#include "gmt/GeometriaEx.h"
#include "SampleMesh.h"

class InstancingScene
    : private gmt::ContextListener
    , private gmt::FrustumListener
    , private gmt::EnableWeakFromThis<gmt::ContextListener>
    , private gmt::EnableWeakFromThis<gmt::FrustumListener>
{
public:
    InstancingScene(gmt::Context *context);
    
    void update();
    void render();

private:
    gmt::Context *context_;
    gmt::Program program_;
    gmt::UniformMat4f viewProjUniform_;
    gmt::Uniform1i textureUniform_;

    gmt::Frustum frustum_;

    SampleMesh mesh_;

    gmt::InputLayout layout_;
    gmt::Texture texture_;

    void onResize(int width, int height) override;
    void onViewProjChanged() override;
};
