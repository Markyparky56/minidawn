#include "MiniDawn.hpp"
// Maybe inspired by UE4... slightly, but no copy-pasting!
// Define a platform independent main function which is called by the various entry points
// Returns an error code back to the entry point, if there are errors
#if defined(PLATFORM_WINDOWS)
int DawnMain(const TCHAR* CmdLine, HINSTANCE hInstance, HINSTANCE hPrevInstance, int nCmdShow)
#else
int DawnMain(const TCHAR* CmdLine)
#endif
{
    int error = 0;
    // Engine PreInit
    error = GMiniDawnEngine.PreInit();
    if (error) return error;
    // Engine Init
    error = GMiniDawnEngine.Init();
    if (error) return error;
    // Loop Engine Tick
    do 
    {
        error = GMiniDawnEngine.Tick();
    } while (GMiniDawnEngine.IsRunning());
    if (error) return error;
    // Engine Exit
    error = GMiniDawnEngine.Exit();
    return error;
}