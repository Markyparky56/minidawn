#include "WindowsApplication.hpp"
#include <algorithm>

WindowsApplication* windowsApplication = nullptr;

static SharedPtr<WindowsWindow> FindWindowByHWND(const std::vector<SharedRef<WindowsWindow>>& Windows, HWND HWnd)
{
    for (auto Window : Windows)
    {
        if (Window->getHwnd() == HWnd)
        {
            return Window;
        }
    }
    return SharedPtr<WindowsWindow>(nullptr); // Failed
}

WindowsApplication* WindowsApplication::CreateApplication(const HINSTANCE InstanceHandle, const HICON IconHandle)
{
    windowsApplication = new WindowsApplication(InstanceHandle, IconHandle);
    return windowsApplication;
}

void WindowsApplication::PumpMessages(const float DeltaTime)
{
    while (true)
    {
        MSG msg;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            break;
        }
    }
}

void WindowsApplication::Tick(const float DeltaTime)
{
    // UE4 uses this to check modifer keys (shift, ctrl, alt, caps)
    // Instead we'll use it to tick our Scene
    scene->Tick(DeltaTime);
    window->FlipBuffers();
}

SharedRef<GenericWindow> WindowsApplication::MakeWindow()
{
    return WindowsWindow::Make();
}

void WindowsApplication::InitialiseWindow(const SharedRef<GenericWindow>& Window, const SharedRef<GenericWindowDefinition>& InDefinition)
{
    const SharedRef<WindowsWindow> windowRef = StaticCastSharedRef<WindowsWindow>(Window);

    //Windows.push_back(windowRef);
    window = windowRef;
    windowRef->Initialise(InDefinition, hInstance);
}

void WindowsApplication::InitialiseScene(const SharedPtr<Scene>& InScene)
{
    scene = InScene;
    scene->Init(input.Get(), renderer.Get());
}

void WindowsApplication::DestroyApplication()
{
    // Don't actually have to destroy anything manually, damn you smart pointers!
}

LRESULT CALLBACK WindowsApplication::AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return windowsApplication->ProcessMessage(hwnd, msg, wParam, lParam);
}

int32_t WindowsApplication::ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
    SharedPtr<WindowsWindow> CurrentWindow = window;//FindWindowByHWND(Windows, hwnd);

    // switch case to see if we need to handle the message
    // if so pass them onto ProcessDeferredMessage
    if (windowsApplication)
    {
        switch (msg)
        {
        case WM_KEYDOWN:
        case WM_KEYUP:
            {
                ProcessDeferredMessage(DeferredWindowsMessage(CurrentWindow, hwnd, msg, wParam, lParam));
                return 0;
            }
        case WM_MOUSEMOVE:
            {
                ProcessDeferredMessage(DeferredWindowsMessage(CurrentWindow, hwnd, msg, wParam, lParam, LOWORD(lParam), HIWORD(lParam)));        
                return 0;
            }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            {
                ProcessDeferredMessage(DeferredWindowsMessage(CurrentWindow, hwnd, msg, wParam, lParam));
                return 0;
            }
        case WM_CLOSE:
            {
                ProcessDeferredMessage(DeferredWindowsMessage(CurrentWindow, hwnd, msg, wParam, lParam));
                return 0;
            }
        case WM_DESTROY:
            {    
                ProcessDeferredMessage(DeferredWindowsMessage(CurrentWindow, hwnd, msg, wParam, lParam));
                return 0;
            }
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int32_t WindowsApplication::ProcessDeferredMessage(const DeferredWindowsMessage & DeferredMessage)
{
    if (/*Windows.size() &&*/ DeferredMessage.NativeWindow.IsValid())
    {
        HWND hWnd = DeferredMessage.hWND;
        uint32_t msg = DeferredMessage.Message;
        WPARAM wParam = DeferredMessage.wParam;
        LPARAM lParam = DeferredMessage.lParam;

        SharedPtr<WindowsWindow> CurrentWindow = DeferredMessage.NativeWindow.Pin();

        switch (msg)
        {
        case WM_KEYDOWN:
        case WM_KEYUP:
            {
                // Pass to the input manager
                return 0;
            }
        case WM_MOUSEMOVE:
            {
                // Pass to the input manager
                return 0;
            }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            {
                // Pass to the input manager
                return 0;
            }
        case WM_CLOSE:
            {
                GMiniDawnEngine.RequestEnd();
                return 0;
            }
        case WM_DESTROY:
            {
                // Hacky due to differences std::vector and UE's TArray
                //std::vector<SharedRef<WindowsWindow>>::const_iterator wnd;
                //for (std::vector<SharedRef<WindowsWindow>>::const_iterator wndIt = Windows.begin();
                //     wndIt != Windows.end(); wndIt++)
                //{
                //    if (DeferredMessage.NativeWindow.HasSameObject(&wndIt->Get()))
                //    {
                //        wnd = wndIt;
                //        break;
                //    }
                //}
                //Windows.erase(wnd);
                window.Reset();
            }
        }
    }
    return 0;
}

WindowsApplication::WindowsApplication(const HINSTANCE HInstance, const HICON IconHandle)
    : hInstance(HInstance)
{
    bool classRegistered = RegisterWindowsClass(hInstance, IconHandle);

    // maybe this should be done by the engine itself?
    input = MakeShareable(new InputSystem);
    renderer = MakeShareable(new LegacyOpenGLRenderer);
}

bool WindowsApplication::RegisterWindowsClass(const HINSTANCE HInstance, const HICON HIcon)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW; // Accept double clicks
    wcex.lpfnWndProc = AppWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = static_cast<HINSTANCE>(HInstance);
    wcex.hIcon = static_cast<HICON>(HIcon);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // Ideally this should be modifiable
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
    wcex.lpszClassName = L"WindowClass";
    wcex.lpszMenuName = NULL;
    wcex.hIconSm = NULL; // static_cast<HICON>(hIconSm);

    return RegisterClassEx(&wcex); // Performance warning
}
