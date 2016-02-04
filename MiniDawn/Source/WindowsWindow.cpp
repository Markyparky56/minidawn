#include "WindowsWindow.hpp"

WindowsWindow::WindowsWindow()
{
}

WindowsWindow::~WindowsWindow()
{
}

void WindowsWindow::Register(void * hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(wcex);
    wcex.style = classStyle;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = static_cast<HINSTANCE>(hInstance);
    wcex.hIcon = static_cast<HICON>(hIcon);
    wcex.hCursor = static_cast<HCURSOR>(hCursor);
    wcex.hbrBackground = static_cast<HBRUSH>(hbrBackground);
    wcex.lpszMenuName = menuName;
    wcex.lpszClassName = className;
    wcex.hIconSm = static_cast<HICON>(hIconSm);

    RegisterClassEx(&wcex);
}

int WindowsWindow::Initialise(void * hInstance, int nCmdShow)
{
    hwnd = static_cast<void*>
            (
                CreateWindow
                (
                    className, 
                    title, 
                    wndStyle, 
                    xPos, 
                    yPos, 
                    width, 
                    height, 
                    static_cast<HWND>(parent), 
                    static_cast<HMENU>(menu), 
                    static_cast<HINSTANCE>(hInstance), 
                    param
                )
            );
    if (!hwnd)
    {
        return FALSE;
    }
    ShowWindow(static_cast<HWND>(hwnd), nCmdShow);
    UpdateWindow(static_cast<HWND>(hwnd));
    return TRUE;
}

void WindowsWindow::setStyle(unsigned int newStyle)
{
    classStyle = newStyle;
}

void WindowsWindow::setIcon(void * newIcon)
{
    hIcon = newIcon;
}

void WindowsWindow::setSmallIcon(void * newIconSm)
{
    hIconSm = newIconSm;
}

void WindowsWindow::setCursor(void * newCursor)
{
    hCursor = newCursor;
}

void WindowsWindow::setBackground(void * newBackground)
{
    hbrBackground = newBackground;
    SetClassLongPtr(static_cast<HWND>(hwnd), GCLP_HBRBACKGROUND, reinterpret_cast<LONG>(newBackground));
    InvalidateRect(static_cast<HWND>(hwnd), 0, TRUE); // Force redraw
}

void WindowsWindow::setMenuName(wchar_t * newMenuName)
{
    menuName = newMenuName;
}

void WindowsWindow::setClassName(wchar_t * newClassName)
{
    className = newClassName;
}

void WindowsWindow::setTitle(wchar_t * newTitle)
{
    title = newTitle;
}

void WindowsWindow::setWndStyle(unsigned long newWndStyle)
{
    wndStyle = newWndStyle;
}

void WindowsWindow::setXPos(int newXPos)
{
    xPos = newXPos;
}

void WindowsWindow::setYPos(int newYPos)
{
    yPos = newYPos;
}

void WindowsWindow::setWidth(int newWidth)
{
    width = newWidth;
}

void WindowsWindow::setHeight(int newHeight)
{
    height = newHeight;
}

void WindowsWindow::setParent(void * newParent)
{
    parent = newParent;
}

void WindowsWindow::setMenu(void * newMenu)
{
    menu = newMenu;
}

void WindowsWindow::setParam(void * newParam)
{
    param = newParam;
}

void WindowsWindow::updateTitle(wchar_t* newTitle)
{

    SetWindowText(static_cast<HWND>(hwnd), newTitle);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, message, wParam, lParam);
}
