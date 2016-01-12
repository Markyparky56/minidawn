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

}