//
//  GeometriaEx.h
//
//  Created by Grigory Shabaganov on 19.08.2017.
//
//

#pragma once

#include <vector>

namespace gmt
{

namespace ext
{

struct TextureData
{
    int width;
    int height;
    std::vector<char> data;
};

bool loadPng(const char *filename, bool hasAlpha, TextureData *data);

}

}

