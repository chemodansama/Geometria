//
//  Context.cpp
//
//  Created by Grigory Shabaganov on 26.07.2017.
//

#include "gmt/render/Context.h"

namespace gmt
{

void Context::onResize(int width, int height)
{
    notify([width, height](auto x){ x->onResize(width, height); });
}

}
