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

private:
    void Update() override;
    void Render() override;
    void Setup() override;

    void SetupCube36(Object* cube);

    std::vector<pObject> objects;
};
