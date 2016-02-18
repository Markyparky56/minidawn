#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <gl\GL.h>
#include <gl\GLU.h>
// UE4's OpenGL headers are far too complicated to replicate, and don't use legacy OpenGL, so
// This is where I start to reimplement the stuff from the labs
// However having the opengl context inside the scene class always seemed a bit odd to me

// Before this diverges entirely from UE4's implementation however, I like that UE4 packages all
// their context variables into a nice handy struct (albeit pruned, due to this being Legacy OpenGL)
struct OpenGLContext
{
    HWND windowHandle;
    HDC deviceContext;
    HGLRC openGLContext;
};

const int ColourDepth = 24;
const int DepthDepth = 16; // Maybe needs a new name?

bool CreatePixelFormat(HDC InHDC);
void ResizeGLWindow(int Width, int Height);
void InitialiseOpenGL(OpenGLContext* Context, int Width, int Height);
