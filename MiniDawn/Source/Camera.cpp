#include "Camera.hpp"

void Camera::Update()
{
    float cosRoll, cosPitch, cosYaw;
    float sinRoll, sinPitch, sinYaw;

    cosRoll     = cosf(roll *PI / 180);
    cosPitch    = cosf(pitch*PI / 180);
    cosYaw      = cosf(yaw  *PI / 180);
    sinRoll     = sinf(roll *PI / 180);
    sinPitch    = sinf(pitch*PI / 180);
    sinYaw      = sinf(yaw  *PI / 180);

    // I have no idea how these work, magic? Probably magic.
    forward.x = sinYaw * cosPitch;
    forward.y = sinPitch;
    forward.z = cosPitch * -cosYaw;

    up.x = -cosYaw * sinRoll - sinYaw * sinPitch * cosRoll;
    up.y = cosPitch * cosRoll;
    up.z = -sinYaw * sinRoll - sinPitch * cosRoll * -cosYaw;

    right = forward.cross(up);

    lookat.x = position.x + forward.x;
    lookat.y = position.y + forward.y;
    lookat.z = position.z + forward.z;
}