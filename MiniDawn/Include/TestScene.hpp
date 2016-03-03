#pragma once
// Engine Includes
#include "Scene.hpp"
#include "LegacyOpenGLRenderer.hpp"

// STL Collections
#include <vector>
#include <unordered_map>

// Additional includes
#include "object.hpp"
#include "Model.h"

struct GenericWindowDefinition;

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

    Vector2 screenCentre, newPos;

    CamRot camRot;
    pCamera camera;

    std::vector<pObject> objects;
    Model model;

    SharedPtr<GenericWindowDefinition> windowDef;
};
