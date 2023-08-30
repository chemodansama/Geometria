//
//  SampleMesh.h
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#pragma once

#include <cstdio>
#include <array>
#include <memory>
#include <chrono>

#include "gmt/gmt.h"
#include "gmt/render/InputLayout.h"

class SampleMesh
{
public:
    SampleMesh(bool instanced = false);

    gmt::InputLayout createInputLayout(const gmt::Program &program) const;
    
    void drawCube() const;
    void drawQuad() const;
    void updateInstances();

private:
    gmt::VertexBuffer vertices_;
    gmt::VertexBuffer indices_;
    std::unique_ptr<gmt::VertexBuffer> instances_;

    std::array<glm::mat4x4, 4> instancesData_;
    float cubeAngle_{ 0.0f };
    std::chrono::high_resolution_clock::time_point updateTime_;
};
