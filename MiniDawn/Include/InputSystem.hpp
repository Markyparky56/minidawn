#pragma once
// TODO: remove STL and Windows dependencies, it'll do for now though
#include "UniquePtr.hpp"
#include <array>
#include <Windows.h>

class InputSystem
{
private:
    mutable struct Mouse {
        uint32_t x, y;
        bool left, right, middle;
    } mouse;

public:
    InputSystem();
    ~InputSystem();
    
    bool const IsKeyDown(uint32_t key) const;
    bool const IsKeyUp(uint32_t key) const;

    void const SetKeyUp(uint32_t key) const;
    void const SetKeyDown(uint32_t key) const;

    void const SetMouseX(uint32_t newX) const;
    void const SetMouseY(uint32_t newY) const;
    void const SetMouseLeft(bool state) const;
    void const SetMouseRight(bool state) const;
    void const SetMouseMiddle(bool state) const;

    uint32_t const GetMouseX() const;
    uint32_t const GetMouseY() const;
    bool const GetMouseLeft() const;
    bool const GetMouseRight() const;
    bool const GetMouseMiddle() const;

    void HandleMouseClick(int msg);

private:
    mutable std::array<bool, 256> keys;
    mutable uint32_t mouseX, mouseY;
};
using pInputSystem = UniquePtr<InputSystem>;
