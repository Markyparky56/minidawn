#pragma once
#include "Camera.hpp"
#include "InputSystem.hpp"
#include "Renderer.hpp"

// Temporary STL unique_ptr, replace with compatible UE4 UniquePtrs when able
#include <memory>
using pCamera = std::unique_ptr<Camera>;

class Scene
{
public:
    Scene()
        : wireframeMode(false)
        , gameTime(0.0)
    {}
    virtual void Init(InputSystem* InInputSystem, Renderer* InRenderer);
    virtual void Tick(float InDeltaTime);

protected:
    virtual void Update();
    virtual void Render();
    virtual void Setup();

    InputSystem* inputSystem;
    Renderer* renderer;

    bool wireframeMode;
    float deltaTime;
    double gameTime;

    CamRot camRot;
    pCamera camera;
};