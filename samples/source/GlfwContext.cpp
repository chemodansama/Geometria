//
//  GlfwContext.cpp
//
//  Created by Grigory Shabaganov on 26.07.2017.
//

#include "GlfwContext.h"

GlfwContext::GlfwContext(int width, int height)
{
    setWidth(width);
    setHeight(height);
}

void GlfwContext::resize(int width, int height)
{
    if (width != this->width() || height != this->height()) {
        onResize(width, height);
        setWidth(width);
        setHeight(height);
    }
}
