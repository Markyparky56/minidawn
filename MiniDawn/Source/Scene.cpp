#include "Scene.hpp"

void Scene::Init(InputSystem * InInputSystem, Renderer * InRenderer, GenericApplication* InParentApp)
{
    inputSystem = InInputSystem;
    renderer = InRenderer;
    parentApp = InParentApp;
    Setup();
}

void Scene::Tick(float InDeltaTime)
{
    deltaTime = InDeltaTime / 1000.0f;
    gameTime += deltaTime;
    Update();
    Render();
}

void Scene::Update()
{
}

void Scene::Render()
{
}

void Scene::Setup()
{
}
