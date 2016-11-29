#pragma once
#include <string>

enum class WindowType
{
    Normal // Who knows? Maybe I'll exapnd this in the future
};

struct GenericWindowDefinition
{
    WindowType Type;

    // Window position
    float X, Y;

    // Width, Height
    float Width, Height;

    // Window Style
    bool HasOSWindowBorder;
    bool AppearsInTaskBar;
    //bool IsTopmostWindow;   //?
    //bool AcceptsInput;      //?
    bool HasCloseButton;
    bool SupportsMinimise;
    bool SupportsMaximise;
    bool IsResizable;
    
    std::wstring Title;
};
