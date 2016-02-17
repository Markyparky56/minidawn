#pragma once
#include <Windows.h>
#include "GenericWindow.hpp"

class WindowsWindow : public GenericWindow
{
public:
    WindowsWindow();
    ~WindowsWindow();

    void Register(void* hInstance) override;
    int Initialise(void* hInstance, int nCmdShow) override;

    void setStyle(unsigned int newStyle) override;
    void setIcon(void* newIcon) override;
    void setSmallIcon(void* newIconSm) override;
    void setCursor(void* newCursor) override;
    void setBackground(void* newBackground) override;
    void setMenuName(wchar_t* newMenuName) override;
    void setClassName(wchar_t* newClassName) override;

    void setTitle(wchar_t* newTitle) override;
    void setWndStyle(unsigned long newWndStyle) override;
    void setXPos(int newXPos) override;
    void setYPos(int newYPos) override;
    void setWidth(int newWidth) override;
    void setHeight(int newHeight) override;
    void setParent(void* newParent) override;
    void setMenu(void* newMenu) override;
    void setParam(void* newParam) override;

    void updateTitle(wchar_t* newTitle) override;


private:

};