#pragma once
#include <cmath>
#include "maths.vector.hpp"

using std::sinf;
using std::cosf;

const double PI = 3.1415926535897932384626433832795;

class Camera
{
public:
    Camera()
        : yaw(0.0f)
        , pitch(0.0f)
        , roll(0.0f)
    {}
    virtual ~Camera()
    {}
    inline void SetPosition(const Vector3& NewPos)  { position = NewPos; }
    inline void SetUp(const Vector3& NewUp)         { up = NewUp; }
    inline void SetYaw(const float NewYaw)          { yaw = NewYaw; }
    inline void SetPitch(const float NewPitch)      { pitch = NewPitch; }
    inline void SetRoll(const float NewRoll)        { roll = NewRoll; }

    inline Vector3 GetPosition()    { return position; }
    inline Vector3 GetLookAt()      { return lookat; }
    inline Vector3 GetUp()          { return up; }
    inline Vector3 GetForward()     { return forward; }
    inline Vector3 GetRight()       { return right; }

    void Update();

protected:
    Vector3 position;
    Vector3 lookat;
    Vector3 up;
    Vector3 right;
    Vector3 forward;
    float yaw, pitch, roll;
};

// Basically a Vector3 without all the maths
struct CamRot
{
    float roll;
    float yaw;
    float pitch;
};