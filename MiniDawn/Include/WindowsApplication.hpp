#pragma once
#include <Windows.h>
#include <vector>
#include "SharedRef.hpp"
#include "GenericApplication.hpp"
#include "WindowsWindow.hpp"
#include "GMiniDawnEngine.hpp"

// We likely won't actually defer messages, but it's a nice way to bundle all the data together instead of passing
// it as separate paramters
struct DeferredWindowsMessage
{
    DeferredWindowsMessage(const SharedPtr<WindowsWindow>& InNativeWindow, HWND InHWnd, uint32_t InMessage, WPARAM InWParam, LPARAM InLParam, int32_t InX=0, int32_t InY=0, uint32_t InRawInputFlags=0) // UE has extra: int32 InX=0, int32 InY=0, uint32 InRawInputFlags=0
        : NativeWindow(InNativeWindow)
        , hWND(InHWnd)
        , Message(InMessage)
        , wParam(InWParam)
        , lParam(InLParam)
        , X(InX)
        , Y(InY)
        , RawInputFlags(InRawInputFlags)
    { }

    // Window that received the message
    WeakPtr<WindowsWindow> NativeWindow;

    // Window handle
    HWND hWND; 

    // Message code
    uint32_t Message;

    // Message data
    WPARAM wParam;
    LPARAM lParam;

    // Mouse coordinates
    int32_t X;
    int32_t Y;
    uint32_t RawInputFlags;
};

class WindowsApplication : public GenericApplication
{
public:
    static WindowsApplication* CreateApplication(const HINSTANCE InstanceHandle, const HICON IconHandle);
    virtual ~WindowsApplication() {};

    void PumpMessages(const float DeltaTime) override; // Deal with all windows messages in the queue
    void Tick(const float DeltaTime) override;
    SharedRef<GenericWindow> MakeWindow() override;
    void InitialiseWindow(const SharedRef<GenericWindow>& Window, const SharedRef<GenericWindowDefinition>& InDefinition) override;
    void DestroyApplication() override;

protected:
    static LRESULT CALLBACK AppWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    int32_t ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
    int32_t ProcessDeferredMessage(const DeferredWindowsMessage& DeferredMessage);

private:
    WindowsApplication(const HINSTANCE HInstance, const HICON IconHandle);
    bool RegisterWindowsClass(const HINSTANCE HInstance, const HICON HIcon);

    HINSTANCE hInstance;

    std::vector<SharedRef<WindowsWindow>> Windows; // Hypothetically possible to have additional windows
};
