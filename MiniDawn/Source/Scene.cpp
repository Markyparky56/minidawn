#include "Scene.hpp"

void Scene::Init(InputSystem * InInputSystem, Renderer * InRenderer)
{
    inputSystem = InInputSystem;
    renderer = InRenderer;
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
