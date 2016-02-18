#pragma once
#include "SharedRef.hpp"
#include "GenericApplication.hpp"
#if defined(PLATFORM_WINDOWS)
    #include "WindowsApplication.hpp"
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

    SharedPtr<GenericApplication> Application;
    // Timer
    // Input handler
    // scenes
    // renderer
};

extern MiniDawnEngine GMiniDawnEngine;
