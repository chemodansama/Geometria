//
//  GlfwContext.h
//
//  Created by Grigory Shabaganov on 26.07.2017.
//

#pragma once

#include "gmt/render/Context.h"

class GlfwContext : public gmt::Context
{
public:
    GlfwContext(int width, int height);
    
    void resize(int width, int height);
};
