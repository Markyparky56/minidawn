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

    // create window
#if defined(PLATFORM_WINDOWS)
    wnd = std::make_unique<WindowsWindow>();
#endif
    //TODO: other window types

    // create input handler

    // create renderer

    // create scene pointer?
    return 0;
}

int MiniDawnEngine::Init()
{
#if defined(PLATFORM_WINDOWS)
    void* hInstance = GetModuleHandle(NULL);

    wnd->setStyle(CS_HREDRAW | CS_VREDRAW);
    wnd->setIcon(0);
    wnd->setCursor(LoadCursor(NULL, IDC_ARROW));
    wnd->setBackground(reinterpret_cast<void*>(CreateSolidBrush(RGB(255, 0, 255))));
    wnd->setClassName(L"WindowClass");
    wnd->setMenuName(NULL);
    wnd->setSmallIcon(0);
    
    wnd->setTitle(L"MiniDawn");
    wnd->setWndStyle(WS_OVERLAPPEDWINDOW);
    wnd->setXPos(CW_USEDEFAULT);
    wnd->setYPos(CW_USEDEFAULT);
    wnd->setWidth(800);
    wnd->setHeight(600);
    wnd->setParent(nullptr);
    wnd->setMenu(nullptr);
    wnd->setParam(nullptr);

    wnd->Register(hInstance);
    if (!wnd->Initialise(hInstance, SW_SHOW))
    {
        exit(FALSE);
    }
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
        return error; 
    }
    error = GEngineLoop.Tick();
    return error;
}

int MiniDawnEngine::Exit()
{
    // Scene shutdown?
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
