#pragma once
#include <memory>
#include <string>
#include "SharedRef.hpp"
#include "GenericWindowDefinition.hpp"

class GenericWindow
{
public:
    GenericWindow();
    virtual ~GenericWindow();

    virtual void Register();
    virtual void Register(void* hInstance);
    virtual void Initialise(const SharedRef<GenericWindowDefinition>& InDefinition, void* HInstance);
    virtual void FlipBuffers();

    inline void* getHwnd() const { return hWnd; }
    virtual void updateTitle(const wchar_t* newTitle);
    SharedPtr<GenericWindowDefinition> GetDef() { return definition; }

protected:
    void* hWnd;
    SharedPtr<GenericWindowDefinition> definition;
    /*unsigned int classStyle;
    void* hIcon;
    void* hIconSm;
    void* hCursor;
    void* hbrBackground;
    wchar_t* menuName;
    wchar_t* className;

    //wchar_t* title;
    /*unsigned long wndStyle;

    int xPos;
    int yPos;
    int width;
    int height;

    /*void* parent;
    void* menu;
    void* param;*/
};
