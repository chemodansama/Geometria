//
//  Config.h
//
//  Created by Grigory Shabaganov on 29.07.2017.
//

#pragma once

namespace samples
{

constexpr inline bool debug()
{
#ifdef NDEBUG
    return false;
#else
    return true;
#endif
}

}
