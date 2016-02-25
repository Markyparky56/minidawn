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

    // create input handler

    // create renderer

    // create scene pointer?
    return 0;
}

int MiniDawnEngine::Init()
{
    GEngineLoop.PreInit();
    GEngineLoop.Init();
    GEngineLoop.AppInit();
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
        return error; 
    }
    error = GEngineLoop.Tick();
    return error;
}

int MiniDawnEngine::Exit()
{    
    GEngineLoop.AppPreExit();
    GEngineLoop.AppExit();
    GEngineLoop.Exit();    
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
