//
//  debug.cpp
//
//  Created by Grigory Shabaganov on 26.08.2017.
//

#include "gmt/Debug.h"

#ifdef __APPLE__
#elif defined(_WIN32)
#include <cassert>
#include <Windows.h>
#include <WinBase.h>
#include <Dbghelp.h>
#endif

namespace gmt
{

namespace debug
{

void printStacktrace()
{
#ifdef __APPLE__
#elif defined(_WIN32)
    unsigned int   i;
    void* stack[100];
    unsigned short frames;
    SYMBOL_INFO* symbol;
    HANDLE process;

    process = GetCurrentProcess();

    SymInitialize(process, nullptr, TRUE);

    frames = CaptureStackBackTrace(0, 100, stack, nullptr);
    symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    assert(symbol);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    for (i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        printf("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, (unsigned int)symbol->Address);
    }

    free(symbol);
#endif
}

}

}
