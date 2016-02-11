#include "GMiniDawnEngineLoop.hpp"

MiniDawnEngineLoop GEngineLoop;

MiniDawnEngineLoop::MiniDawnEngineLoop()
{
}

MiniDawnEngineLoop::~MiniDawnEngineLoop()
{
}

int MiniDawnEngineLoop::Tick()
{
    int error = 0;
    while (true)
    {
// TODO: Move this to an application class

//        // Process system messages
//#if defined(PLATFORM_WINDOWS)
//        MSG msg;
//        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//        {
//            if (msg.message = WM_QUIT)
//            {
//                break;
//            }
//            switch (msg.message)
//            {
//            case WM_CREATE:
//            case WM_SIZE:
//            case WM_ERASEBKGND:
//                break;
//
//            case WM_KEYDOWN:
//            case WM_KEYUP:
//                // Pass to Input Handler
//                break;
//
//            case WM_MOUSEMOVE:
//                // Pass to Input Handler
//                break;
//
//            case WM_LBUTTONDOWN:
//            case WM_RBUTTONDOWN:
//            case WM_MBUTTONDOWN:
//            case WM_LBUTTONUP:
//            case WM_RBUTTONUP:
//            case WM_MBUTTONUP:
//                // Pass to Input Handler
//                break;
//            case WM_DESTROY:
//                PostQuitMessage(0);
//                break;
//            default:
//                DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
//                break;
//            }
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//#endif
//        // TODO: Other platforms
//        else
//        {
//            // timer tick
//            // Handle input?
//            // Render
//            
//        }
    }
    return 0;
}
