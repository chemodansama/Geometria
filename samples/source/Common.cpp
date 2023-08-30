//
//  Common.cpp
//
//  Created by Grigory Shabaganov on 03.09.2017.
//

#include "Common.h"

#include <cassert>

#include "gmt/GeometriaEx.h"

gmt::Texture loadCheckerTexture()
{
    gmt::ext::TextureData textureData;
    auto success = gmt::ext::loadPng(R"(assets/textures/checker.png)", true, &textureData);
    assert(success);

    gmt::Texture texture;
    success = gmt::TextureEditor{}
            .setData(textureData.width, textureData.height, textureData.data.data())
            .commit(&texture, true);
    assert(success);
    
    return texture;
}
