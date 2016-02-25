#pragma once
#include "Camera.hpp"

class InputSystem;
class Renderer;

// Temporary STL unique_ptrs, replace with compatible UE4 UniquePtrs when able
#include <memory>
using pCamera = std::unique_ptr<Camera>;

//#include "object.hpp"

class Scene
{
public:
    void Init(InputSystem* InInputSystem, Renderer* InRenderer);
    void Tick(float InDeltaTime);

protected:
    virtual void Update();
    virtual void Render();

    InputSystem* inputSystem;
    Renderer* renderer;

    bool wireframeMode;
    float deltaTime;
    double gameTime;

    CamRot camRot;
};