//
//  path.cpp
//
//  Created by Grigory Shabaganov on 26.08.2017.
//

#include "gmt/Path.h"

#ifdef __APPLE__
#elif defined(_WIN32)
#include <Windows.h>
#include <WinBase.h>
#include <Dbghelp.h>
#endif

namespace gmt
{

namespace path
{

std::string getExecutablePath()
{
#ifdef __APPLE__
    return std::string{};
#elif defined(_WIN32)
    TCHAR szPath[MAX_PATH];
    
    if (auto size = GetModuleFileName(0, szPath, MAX_PATH)) {
        std::string filename(szPath);
        return filename.substr(0, filename.find_last_of("\\/") + 1);
    } else {
        return "";
    }
#else
    return std::string{};
#endif
}

std::string collapse(const std::string& filename)
{
    std::string result;

    bool skipFirstSlash = false;
    static const std::string findme{ "/../" };
    for (size_t i = 0u; i < filename.size();) {
        if (filename.compare(i, findme.size(), findme) == 0) {
            while (!result.empty() && result.back() != '/') {
                result.pop_back();
            }
            if (!result.empty()) {
                result.pop_back();
            } else {
                skipFirstSlash = true;
            }

            i += findme.size() - 1;
        } else {
            if (skipFirstSlash && filename[i] == '/') {
                skipFirstSlash = false;
            } else {
                result += filename[i];
            }
            i += 1;
        }
    }

    return result;
}

}

}
