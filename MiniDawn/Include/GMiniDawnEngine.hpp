#pragma once
#include <memory>
#include "GenericApplication.hpp"
#if defined(PLATFORM_WINDOWS)
    #include "WindowsWindow.hpp"
#endif
#include "GMiniDawnEngineLoop.hpp"

class MiniDawnEngineLoop;

class MiniDawnEngine
{
public:
    MiniDawnEngine();
    ~MiniDawnEngine();

    int PreInit();
    int Init();
    int Tick();
    int Exit();

    bool IsRunning();
    void RequestEnd();
private:
    bool isRunning;
    bool requestingEnd;

    pWindow wnd; 
    // Timer
    // Input handler
    // scenes
    //renderer

};
