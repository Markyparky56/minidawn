#pragma once
#include "Scene.hpp"
#include "LegacyOpenGLRenderer.hpp"

// STL Collections
#include <vector>
#include <unordered_map>

#include "object.hpp"

class TestScene : public Scene
{
public:
    TestScene()
    {}
    ~TestScene()
    {}
    //virtual void Init(InputSystem* InInputSystem, Renderer* InRenderer);
    //void Tick(float InDeltaTime) override;

private:
    void Update() override;
    void Render() override;
    void Setup() override;

    void SetupCube36(Object* cube);

    std::vector<pObject> objects;
};
