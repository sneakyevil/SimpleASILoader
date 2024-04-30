#pragma once
#include <Windows.h>

namespace Core
{
    __forceinline LPSTR GetFormattedMessageID(DWORD p_MsgID)
    {
        LPSTR pBuffer = nullptr;
        
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 0, p_MsgID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pBuffer, 0, 0);
    
        return pBuffer;
    }

    __forceinline LPSTR GetFormattedMessage(const char* p_Message, ...)
    {
        LPSTR pBuffer = nullptr;

        va_list vaArgs;
        va_start(vaArgs, p_Message);

        FormatMessageA(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER, p_Message, 0, 0, (LPSTR)&pBuffer, 0, &vaArgs);

        va_end(vaArgs);

        return pBuffer;
    }
}