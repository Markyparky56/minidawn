#include <Windows.h>

extern int DawnMain(const TCHAR* CmdLine, HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nCmdShow)
{
    const TCHAR* CmdLine = GetCommandLine();
    int Error = DawnMain(CmdLine, hInstance, hPrevInstance, nCmdShow);
    if (Error)
    {
        //GErrorHandler stuff?
    }
}