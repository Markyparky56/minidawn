#pragma once
#if defined(PLATFORM_WINDOWS)
    #include <Windows.h>
#endif
#include <thread>
class MiniDawnEngineLoop
{
public:
    MiniDawnEngineLoop();
    ~MiniDawnEngineLoop();

    int Tick();

private:

} GEngineLoop; // Shorter name for sanity