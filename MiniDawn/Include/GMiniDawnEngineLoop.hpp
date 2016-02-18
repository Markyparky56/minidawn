#pragma once
#if defined(PLATFORM_WINDOWS)
    #include <Windows.h>
#endif
#include <thread>
#include "GMiniDawnEngine.hpp"

class MiniDawnEngine;

class MiniDawnEngineLoop
{
public:
    MiniDawnEngineLoop();
    ~MiniDawnEngineLoop();

    int PreInit();
    int Init();
    int Tick();
    void Exit();

    static bool AppInit();
    static void AppPreExit();
    static void AppExit();
    
private:
    static SharedPtr<GenericApplication> application;
    static SharedPtr<GenericWindowDefinition> windowDef;
    // Timer
    // Input handler
    // scenes
    // renderer
};

extern MiniDawnEngineLoop GEngineLoop;
