//
//  ShaderTypes.h
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 coords;
    glm::vec3 color;
    glm::vec2 texcoords;
};
