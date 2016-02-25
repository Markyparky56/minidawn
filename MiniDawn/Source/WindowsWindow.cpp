#include "WindowsWindow.hpp"

WindowsWindow::WindowsWindow()
{
}

WindowsWindow::~WindowsWindow()
{
}

void WindowsWindow::Initialise(const SharedRef<GenericWindowDefinition> & InDefinition, void * HInstance)
{
    definition = InDefinition;
    uint32_t WindowExStyle = 0;
    uint32_t WindowStyle;

    // Not sure why UE4 chose to use floats for their initial values, but it could have some interesting applications
    const float XInit = definition->X;
    const float YInit = definition->Y;
    
    const float WidthInit = definition->Width;
    const float HeightInit = definition->Height;

    int32_t X = (static_cast<int>(XInit) << 0);
    int32_t Y = (static_cast<int>(YInit) << 0);
    int32_t WindowWidth = (static_cast<int>(WidthInit) << 0);
    int32_t WindowHeight = (static_cast<int>(HeightInit) << 0);

    if (!definition->HasOSWindowBorder)
    {
        WindowExStyle = WS_EX_WINDOWEDGE;
        WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        if (definition->AppearsInTaskBar)
        {
            WindowExStyle |= WS_EX_APPWINDOW;
        }
        else
        {
            WindowExStyle |= WS_EX_TOOLWINDOW;
        }
    }
    else
    {
        WindowExStyle = WS_EX_APPWINDOW;
        WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

        if (definition->SupportsMaximise)
        {
            WindowStyle |= WS_MAXIMIZEBOX;
        }
        if (definition->SupportsMinimise)
        {
            WindowStyle |= WS_MINIMIZEBOX;
        }
        if (definition->IsResizable)
        {
            WindowStyle |= WS_THICKFRAME;
        }
        else
        {
            WindowStyle |= WS_BORDER;
        }
        // UE4 does some fiddling with the window dimensions here, accounting for the border sizes
        // I'll skip this for now
    }

    hWnd = CreateWindowEx(
            WindowExStyle,
            L"WindowClass",
            definition->Title.c_str(),
            WindowStyle,
            X, Y, WindowWidth, WindowHeight,
            NULL, NULL, static_cast<HINSTANCE>(HInstance), NULL);

    if (hWnd == NULL) // Window creation failed, uh-oh!
    {
        // Error handling here
        return;
    }

    GLContext = MakeShareable(new OpenGLContext);
    GLContext->windowHandle = static_cast<HWND>(hWnd);
    GetClientRect(GLContext->windowHandle, &GLContext->screenRect);
    InitialiseOpenGL(GLContext.Get());

    // OpenGL Settings
    glShadeModel(GL_SMOOTH);
    glClearColor(0.39f, 0.58f, 93.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void WindowsWindow::FlipBuffers()
{
    if (GLContext.IsValid())
    {
        SwapBuffers(GLContext->deviceContext);
    }
}

void WindowsWindow::updateTitle(wchar_t* newTitle)
{
    SetWindowText(static_cast<HWND>(hWnd), newTitle);
}

SharedRef<WindowsWindow> WindowsWindow::Make()
{
    return MakeShareable(new WindowsWindow());
}
