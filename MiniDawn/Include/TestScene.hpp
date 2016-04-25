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
        Vector3 diffLhs = lhs->GetTransform().GetPosition() - cam->GetPosition();
        Vector3 diffRhs = rhs->GetTransform().GetPosition() - cam->GetPosition();
        float distanceLhs = std::sqrtf(diffLhs.dot(diffLhs));
        float distanceRhs = std::sqrtf(diffRhs.dot(diffRhs));

        if (lhs->UsingTransparency() &&  rhs->UsingTransparency()
        || !lhs->UsingTransparency() && !rhs->UsingTransparency())
        {
            return distanceLhs < distanceRhs;
        }
        else
        {
            if (lhs->UsingTransparency())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
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

    void SetupObjects();

    float buttonPressGracePeriod;
    bool lockMouse;
    float lockMouseLastChangeTime;
    float wireframeModeLastChangeTime;

    bool rotationActive;
    float rotationActiveLastChangeTime;

    Vector2 screenCentre, newPos;

    CamRot camRot;
    UniquePtr<Camera> camera;
    UniquePtr<Camera> fixedCam;
    float cameraSpeed;
    bool fixedCamEnabled;
    float fixedCamEnabledLastChangeTime;

    std::vector<spObject> objects;
    std::unordered_map<std::wstring, spObject> objectMap;

    std::priority_queue<spObject, std::deque<spObject>, DrawQueueSort> drawQueue;
    //UniquePtr<Skybox> skybox;
    UniquePtr<Skysphere> skypshere;

    SharedPtr<GenericWindowDefinition> windowDef;
};
