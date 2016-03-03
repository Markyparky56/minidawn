#pragma once
// Engine Includes
#include "Scene.hpp"
#include "LegacyOpenGLRenderer.hpp"

// STL Collections
#include <vector>
#include <unordered_map>

// Additional includes
#include "PrimitiveObjects.hpp"
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

    float buttonPressGracePeriod;
    bool lockMouse;
    float lockMouseLastChangeTime;
    float wireframeModeLastChangeTime;

    Vector2 screenCentre, newPos;

    CamRot camRot;
    pCamera camera;
    float cameraSpeed;

    //std::vector<pObject> objects;
    // TODO: Rewrite Object class to use Model
    Model model;
    PrimitiveCube cube;

    SharedPtr<GenericWindowDefinition> windowDef;
};
