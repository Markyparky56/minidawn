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
#include <queue>

#include <cassert>

struct GenericWindowDefinition;

class DrawQueueSort
{
public:
    static Camera* cam;

    bool operator()(spObject& lhs, spObject& rhs)
    {
        assert(cam);
        Vector3 diffLhs = lhs->GetPosition() - cam->GetPosition();
        Vector3 diffRhs = rhs->GetPosition() - cam->GetPosition();
        float distanceLhs = std::sqrtf(diffLhs.dot(diffLhs));
        float distanceRhs = std::sqrtf(diffRhs.dot(diffRhs));

        return distanceLhs < distanceRhs;
    }
};

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
    UniquePtr<Camera> camera;
    float cameraSpeed;

    std::vector<spObject> objects;
    std::priority_queue<spObject, std::deque<spObject>, DrawQueueSort> drawQueue;
    //UniquePtr<Skybox> skybox;
    UniquePtr<Skysphere> skypshere;

    SharedPtr<GenericWindowDefinition> windowDef;
};
