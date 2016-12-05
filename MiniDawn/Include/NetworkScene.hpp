#pragma once
// Engine Include
#include "NetworkSystem.hpp"
#include "Scene.hpp"
#include "LegacyOpenGLRenderer.hpp" // Really should get around to setting up a less outdated renderer
#include "PrimitiveObjects.hpp"
#include "object.hpp"
#include "Skysphere.hpp"

// STL Collections
#include <vector>
#include <unordered_map>
#include <queue>
#include <array>

#include <cassert>
#include <algorithm>

struct GenericWindowDefinition;

// TODO: Move to own file, since this is now duplicated across scenes
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

        if (lhs->UsingTransparency() && rhs->UsingTransparency()
            || !lhs->UsingTransparency() && !rhs->UsingTransparency())
        {
            return distanceLhs < distanceRhs;
        }
        else
        {
            if (lhs->UsingTransparency()) // Put the transparent object after non transparent object in the queue
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

class NetworkScene : public Scene
{
public:
    NetworkScene() : playersSorted(false) {}
    ~NetworkScene() {}

private:
    void Update() override;
    void Render() override;
    void Setup() override;

    void SetupObjects();
    void SortPlayersById();

    float buttonPressGracePeriod;
    bool lockMouse;
    float lockMouseLastChangeTime;
    float wireframeModeLastChangeTime;

    Vector2 screenCentre, newPos;

    YawPitchRoll camRot, playerRot;
    UniquePtr<Camera> camera, firstPersonCam;
    float playerSpeed;
    uint8_t playerId;
    
    std::vector<spObject> objects;
    std::vector<spObject> players;
    std::array<Vector3, 16> playerDirections; // This is where having a bespoke "player" class would begin to be useful
    bool playersSorted;
    std::unordered_map<std::wstring, spObject> objectMap;

    std::priority_queue<spObject, std::deque<spObject>, DrawQueueSort> drawQueue;
    UniquePtr<Skysphere> skysphere;

    SharedPtr<GenericWindowDefinition> windowDef;

    pNetworkSystem networkSystem;
};
