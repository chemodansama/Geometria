//
//  assets.h
//
//  Created by Grigory Shabaganov on 26.08.2017.
//

#pragma once

#include <string>
#include <fstream>
#include "gmt/utils.h"

namespace gmt
{

namespace assets
{

template <typename Container>
Container load(const std::string &filename);

}

}

// Implementation

namespace gmt
{

namespace assets
{

template <typename Container>
Container load(const std::string &filename)
{
    std::ifstream t{ filename };
    Container data;
    if (!t.good()) {
        utils::logf("Failed to read data from {}", filename);
        return data;
    }

    t.seekg(0, std::ios::end);
    data.reserve(static_cast<size_t>(t.tellg()));
    t.seekg(0, std::ios::beg);

    data.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());

    return data;
}

}

}
