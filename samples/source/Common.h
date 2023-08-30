//
//  Common.h
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#pragma once

#include <limits>

#include "gmt/render/Texture.h"

template <typename T>
struct PrimitiveRestartIndex
{
    static constexpr T value = std::numeric_limits<T>::max();
};

gmt::Texture loadCheckerTexture();
