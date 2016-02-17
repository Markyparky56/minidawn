#include "WindowsApplication.hpp"

WindowsApplication* windowsApplication = nullptr;

WindowsApplication * WindowsApplication::CreateApplication(const HINSTANCE InstanceHandle, const HICON IconHandle)
{
    windowsApplication = new WindowsApplication(InstanceHandle, IconHandle);
    return windowsApplication;
}

LRESULT CALLBACK WindowsApplication::AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return windowsApplication->ProcessMessage(hwnd, msg, wParam, lParam);
}

int32_t WindowsApplication::ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
    // switch case to see if we need to handle the message
    // if so process them
    if (windowsApplication)
    {
        switch (msg)
        {
        case WM_SYSKEYDOWN:
            {
                // Alt-F4 or Alt+Space
                if (wParam != VK_F4 && wParam != VK_SPACE)
                {
                    
                }
            }
        case WM_KEYDOWN:
        case WM_SYSKEYUP:
        }

    }
}
