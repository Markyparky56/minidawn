#pragma once
// Source: http://stackoverflow.com/questions/1149620/how-to-write-to-the-output-window-in-visual-studio
#include <cstdarg>
#include <cstdio>
#include <Windows.h>

inline void DebugOutput(const wchar_t* Format, ...)
{
#ifdef _DEBUG
    wchar_t buff[1024];
    va_list arg;
    va_start(arg, Format);
    _vsnwprintf_s(buff, sizeof(buff), Format, arg); // Really ugly function name
    va_end(arg);

    OutputDebugString(buff);
#endif
}