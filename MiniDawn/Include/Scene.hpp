#pragma once
#include "Camera.hpp"
#include "InputSystem.hpp"
#include "Renderer.hpp"
#include "SharedRef.hpp"

// Temporary STL unique_ptr, replace with compatible UE4 UniquePtrs when able
#include "UniquePtr.hpp"
using pCamera = UniquePtr<Camera>;

class GenericApplication;

class Scene
{
public:
    Scene()
        : wireframeMode(false)
        , gameTime(0.0)
    {}
    virtual void Init(InputSystem* InInputSystem, Renderer* InRenderer, GenericApplication* InParentApp);
    virtual void Tick(float InDeltaTime);

protected:
    virtual void Update();
    virtual void Render();
    virtual void Setup();

    GenericApplication* parentApp;

    InputSystem* inputSystem;
    Renderer* renderer;

    bool wireframeMode;
    float deltaTime;
    double gameTime;
};
