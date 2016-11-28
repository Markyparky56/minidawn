#include <Windows.h>
#include <cstdio>
#include <iostream>

extern int DawnMain(const TCHAR* CmdLine, HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nCmdShow)
{
    // This'll be useful for a slightly less weird debug output when dealing with network messages
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    const TCHAR* CmdLine = GetCommandLine();
    int Error = DawnMain(CmdLine, hInstance, hPrevInstance, nCmdShow);
    if (Error)
    {
        //GErrorHandler stuff?
    }
    return Error;
}