#include "OpenGLWindows.hpp"

bool CreatePixelFormat(HDC InHDC)
{
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    int pixelFormat;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;   // Standard mask
    pfd.iPixelType = PFD_TYPE_RGBA;     // RGB and alpha
    pfd.cColorBits = ColourDepth;       // Defined as 24
    pfd.cDepthBits = DepthDepth;        // Defined as 16
    pfd.cAccumBits = 0;                 // Nothing for accumulation (no idea)
    pfd.cStencilBits = 0;               // Nothing for stencil (still no idea)

    if ((pixelFormat = ChoosePixelFormat(InHDC, &pfd)) == false)
    {
        MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
        return false;
    }

    if (SetPixelFormat(InHDC, pixelFormat, &pfd) == false)
    {
        MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
        return false;
    }

    return true;
}

void ResizeGLWindow(int Width, int Height)
{
    if (Height == 0) // Prevent Divide by Zero!
    {
        Height = 1;
    }

    glViewport(0, 0, Width, Height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculate aspect ratio
#ifdef _DEBUG
    gluPerspective(90.0f, static_cast<GLfloat>(Width) / static_cast<GLfloat>(Height), 0.001f, 150.0f);
#else
    gluPerspective(90.0f, static_cast<GLfloat>(Width) / static_cast<GLfloat>(Height), 0.01f, 150.0f);
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void InitialiseOpenGL(OpenGLContext* Context)
{
    Context->deviceContext = GetDC(Context->windowHandle);
    if (!CreatePixelFormat(Context->deviceContext))
    {
        PostQuitMessage(0);
    }

    Context->openGLContext = wglCreateContext(Context->deviceContext);
    wglMakeCurrent(Context->deviceContext, Context->openGLContext);

    ResizeGLWindow(Context->screenRect.right, Context->screenRect.bottom);
}
