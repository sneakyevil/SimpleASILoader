#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace core
{
    __forceinline LPSTR GetFormattedMessageID(DWORD msg_id)
    {
        LPSTR buf = 0;
        
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM, 0, msg_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buf, 0, 0);
    
        return buf;
    }

    __forceinline LPSTR GetFormattedMessage(const char* msg, ...)
    {
        LPSTR buf = 0;

        va_list args;
        va_start(args, msg);

        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, msg, 0, 0, (LPSTR)&buf, 0, &args);

        va_end(args);

        return buf;
    }
}