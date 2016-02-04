#include "GMiniDawnEngine.hpp"

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
    return 0;
}

int MiniDawnEngine::Init()
{
    return 0;
}

int MiniDawnEngine::Tick()
{
    return 0;
}

int MiniDawnEngine::Exit()
{
    return 0;
}

bool MiniDawnEngine::IsRunning()
{
    return isRunning;
}
