//
//  GeometriaEx.cpp
//
//  Created by Grigory Shabaganov on 19.08.2017.
//

#include "gmt/GeometriaEx.h"

#include <png.h>

namespace gmt
{

namespace ext
{

bool loadPng(const char *filename, bool hasAlpha, TextureData *data)
{
    png_image image;
    image.version = PNG_IMAGE_VERSION;
    image.opaque = nullptr;
    png_image_begin_read_from_file(&image, filename);
    image.format = hasAlpha ? PNG_FORMAT_RGBA : PNG_FORMAT_RGB;
    auto bytesPerPel = hasAlpha ? 4 : 3;
    png_color color{ 0 };
    
    data->width = image.width;
    data->height = image.height;
    
    data->data.resize(bytesPerPel * image.width * image.height);
    png_image_finish_read(&image, &color, data->data.data(), 0, nullptr);
    if (image.warning_or_error)
    {
        return false;
    }
    png_image_free(&image);

    return true;
}

}

}
