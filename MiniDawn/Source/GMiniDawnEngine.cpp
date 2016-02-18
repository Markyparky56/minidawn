#include "GMiniDawnEngine.hpp"

extern MiniDawnEngineLoop GEngineLoop;
MiniDawnEngine GMiniDawnEngine;

// We don't want to do anything in the constructor and destructor so we can
// manage the creation and destruction of objects manually via init/exit
MiniDawnEngine::MiniDawnEngine()
{
}

MiniDawnEngine::~MiniDawnEngine()
{
}

int MiniDawnEngine::PreInit()
{
    isRunning = false;
    // Init timer

    // create Application
#if defined(PLATFORM_WINDOWS)
    Application = MakeShareable(WindowsApplication::CreateApplication(GetModuleHandle(NULL), NULL));
#endif

    // create input handler

    // create renderer

    // create scene pointer?
    return 0;
}

int MiniDawnEngine::Init()
{
    // This should be moved to the application
#if defined(PLATFORM_WINDOWS)
    //Application->
#endif
    // Init Scene?
    return 0;
}

int MiniDawnEngine::Tick()
{
    int error = 0;
    if (!isRunning && !requestingEnd) 
    { 
        isRunning = true; 
    }
    else if (requestingEnd) 
    { 
        isRunning = false; 
        error = Exit(); // Maybe?
        return error; 
    }
    error = GEngineLoop.Tick();
    return error;
}

int MiniDawnEngine::Exit()
{
    // Scene shutdown?
    // Application shutdown?
    // Loop cleanup?
    return 0;
}

bool MiniDawnEngine::IsRunning()
{
    return isRunning;
}

void MiniDawnEngine::RequestEnd()
{
    requestingEnd = true;
}
