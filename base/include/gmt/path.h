//
//  path.h
//
//  Created by Grigory Shabaganov on 26.08.2017.
//

#pragma once

#include <string>

namespace gmt
{

namespace path
{

std::string getExecutablePath();
std::string collapse(const std::string &filename);

}

}
