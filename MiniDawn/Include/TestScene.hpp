#pragma once
// Engine Includes
#include "Scene.hpp"
#include "LegacyOpenGLRenderer.hpp"
#include "PrimitiveObjects.hpp"
#include "Object.hpp"
#include "Skysphere.hpp"

// STL Collections
#include <vector>
#include <unordered_map>

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

    std::vector<spObject> objects;
    //UniquePtr<Skybox> skybox;
    UniquePtr<Skysphere> skypshere;

    SharedPtr<GenericWindowDefinition> windowDef;
};
