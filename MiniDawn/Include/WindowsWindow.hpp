#pragma once
#include <Windows.h>
#include "GenericWindow.hpp"
#include "OpenGLWindows.hpp"

class WindowsWindow : public GenericWindow
{
public:
    WindowsWindow();
    ~WindowsWindow();

    void Initialise(const SharedRef<GenericWindowDefinition> & InDefinition, void * HInstance) override;
    void FlipBuffers() override;

    void updateTitle(wchar_t* newTitle) override;

    static SharedRef<WindowsWindow> Make();

private:
    SharedPtr<OpenGLContext> GLContext;
};
