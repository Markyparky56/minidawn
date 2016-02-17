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

    int Tick();

private:

};
