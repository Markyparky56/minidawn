#pragma once
#include "maths.vector.hpp"
#include "Object.hpp"
#include "Model.h"
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Skysphere.hpp"
#include <vector>

class Renderer
{
public:
    Renderer()
        : cam(nullptr)
    {}
    virtual ~Renderer()
    {}

    virtual void Initialise() {}
    virtual void Shutdown() {}

    virtual void StartDrawing() {}
    virtual void EndDrawing() {}

    virtual void DrawObject(const Object& InObject) {}
    virtual void DrawModel(const Model& InModel) {}
    virtual void DrawSkybox(const Skybox& InSkybox) {}
    virtual void DrawSkysphere(const Skysphere& InSkysphere) {}


    virtual void Translate(const Vector3& Trans) {}
    virtual void Rotate(const float Deg, const Vector3& Rot) {}
    virtual void Scale(const Vector3& Scale) {}

    inline void SetCamera(Camera* NewCamPtr) { cam = NewCamPtr; }
    virtual void Look() {}

protected:
    Camera* cam;
};
