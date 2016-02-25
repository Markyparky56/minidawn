#include "inputsystem.hpp"

InputSystem::InputSystem()
{
    keys.fill(false);
}

InputSystem::~InputSystem()
{
}

bool const InputSystem::IsKeyDown(uint32_t key) const 
{
    return keys[key];
}

bool const InputSystem::IsKeyUp(uint32_t key) const
{
    return keys[key];
}

void const InputSystem::SetKeyUp(uint32_t key) const
{
    keys[key] = false;
}

void const InputSystem::SetKeyDown(uint32_t key) const
{
    keys[key] = true;
}

void const InputSystem::SetMouseX(uint32_t newX) const
{
    mouse.x = newX;
}

void const InputSystem::SetMouseY(uint32_t newY) const
{
    mouse.y = newY;
}

void const InputSystem::SetMouseLeft(bool state) const
{
    mouse.left = state;
}

void const InputSystem::SetMouseRight(bool state) const
{
    mouse.right = state;
}

void const InputSystem::SetMouseMiddle(bool state) const
{
    mouse.middle = state;
}

uint32_t const InputSystem::GetMouseX() const
{
    return mouse.x;
}

uint32_t const InputSystem::GetMouseY() const
{
    return mouse.y;
}

bool const InputSystem::GetMouseLeft() const
{
    return mouse.left;
}

bool const InputSystem::GetMouseRight() const
{
    return mouse.right;
}

bool const InputSystem::GetMouseMiddle() const
{
    return mouse.middle;
}
