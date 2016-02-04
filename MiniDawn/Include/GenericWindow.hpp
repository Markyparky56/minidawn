#pragma once
#include <memory>
#include <string>

class GenericWindow
{
public:
    GenericWindow();
    virtual ~GenericWindow();

    virtual void Register();
    virtual void Register(void* hInstance);
    virtual bool Initialise(void* hInstance, int nCmdShow);

    // TODO: Make generic... eventually, this'll do for windows for now
    virtual void setStyle(unsigned int newStyle);
    virtual void setIcon(void* newIcon);
    virtual void setSmallIcon(void* newIconSm);
    virtual void setCursor(void* newCursor);
    virtual void setBackground(void* newBackground);
    virtual void setMenuName(wchar_t* newMenuName);
    virtual void setClassName(wchar_t* newClassName);

    virtual void setTitle(wchar_t* newTitle);
    virtual void setWndStyle(unsigned long newWndStyle);
    virtual void setXPos(int newXPos);
    virtual void setYPos(int newYPos);
    virtual void setWidth(int newWidth);
    virtual void setHeight(int newHeight);
    virtual void setParent(void* newParent);
    virtual void setMenu(void* newMenu);
    virtual void setParam(void* newParam);

    inline const void* const getHwnd() { return &hwnd; }
    virtual void updateTitle(wchar_t* newTitle);

protected:
    void* hwnd;

    unsigned int classStyle;
    void* hIcon;
    void* hIconSm;
    void* hCursor;
    void* hbrBackground;
    wchar_t* menuName;
    wchar_t* className;

    wchar_t* title;
    unsigned long wndStyle;

    int xPos;
    int yPos;
    int width;
    int height;

    void* parent;
    void* menu;
    void* param;
};