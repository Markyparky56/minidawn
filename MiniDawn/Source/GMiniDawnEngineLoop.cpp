#include "GMiniDawnEngineLoop.hpp"

extern MiniDawnEngine GMiniDawnEngine;
MiniDawnEngineLoop GEngineLoop;

SharedPtr<GenericApplication> MiniDawnEngineLoop::application = nullptr;
SharedPtr<GenericWindowDefinition> MiniDawnEngineLoop::windowDef = nullptr;

MiniDawnEngineLoop::MiniDawnEngineLoop()
{
}

MiniDawnEngineLoop::~MiniDawnEngineLoop()
{
}

int MiniDawnEngineLoop::PreInit()
{
    // create Application
#if defined(PLATFORM_WINDOWS)
    application = MakeShareable(WindowsApplication::CreateApplication(GetModuleHandle(NULL), NULL));
#endif
    return 0;
}

int MiniDawnEngineLoop::Init()
{
    return 0;
}

int MiniDawnEngineLoop::Tick()
{
    int error = 0;
    application->PumpMessages(0.0f); // Replace with delta time
    return 0;
}

void MiniDawnEngineLoop::Exit()
{
}

bool MiniDawnEngineLoop::AppInit()
{
    // This should be moved to the application
#if defined(PLATFORM_WINDOWS)
    SharedRef<GenericWindow> wnd = application->MakeWindow();
    windowDef = SharedPtr<GenericWindowDefinition>(new GenericWindowDefinition);
    windowDef->AppearsInTaskBar = true;
    windowDef->HasCloseButton = true;
    windowDef->HasOSWindowBorder = true;
    windowDef->Height = 600;
    windowDef->Width = 800;
    windowDef->IsResizable = false;
    windowDef->SupportsMaximise = false;
    windowDef->SupportsMinimise = true;
    windowDef->Title = L"MiniDawn Window";
    windowDef->Type = WindowType::Normal;
    windowDef->X = CW_USEDEFAULT;
    windowDef->Y = CW_USEDEFAULT;
    application->InitialiseWindow(wnd, windowDef.ToSharedRef());
    ShowWindow(static_cast<HWND>(wnd->getHwnd()), SW_SHOW);
    UpdateWindow(static_cast<HWND>(wnd->getHwnd()));
#endif
    return false;
}

void MiniDawnEngineLoop::AppPreExit()
{
}

void MiniDawnEngineLoop::AppExit()
{
}
