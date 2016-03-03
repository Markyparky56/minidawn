#include "GMiniDawnEngineLoop.hpp"

#include "TestScene.hpp"

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
    timer.Initialize();
    return 0;
}

int MiniDawnEngineLoop::Init()
{
    return 0;
}

bool MiniDawnEngineLoop::AppInit()
{
#if defined(PLATFORM_WINDOWS)
    SharedRef<GenericWindow> wnd = application->MakeWindow();
    windowDef = SharedPtr<GenericWindowDefinition>(new GenericWindowDefinition);
    windowDef->AppearsInTaskBar = true;
    windowDef->HasCloseButton = true;
    windowDef->HasOSWindowBorder = true;
    windowDef->Width = 1280;
    windowDef->Height = 720;    
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

    // Create Scene
    SharedPtr<Scene> scene = MakeShareable(new TestScene);
    application->InitialiseScene(scene);

    return false;
}

int MiniDawnEngineLoop::Tick()
{
    int error = 0;

    timer.Frame();
    float deltaTime = timer.GetTime();

    application->PumpMessages(deltaTime);
    application->Tick(deltaTime);
    return 0;
}

void MiniDawnEngineLoop::Exit()
{
}

void MiniDawnEngineLoop::AppPreExit()
{
}

void MiniDawnEngineLoop::AppExit()
{
}
