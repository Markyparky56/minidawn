#pragma once
#include "maths.vector.hpp"
#include "object.hpp"
#include "Model.h"
#include "Camera.hpp"
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

    virtual void DrawObject(Object& Obj) {}
    virtual void DrawModel(Model& Model) {}

    virtual void Translate(Vector3& Trans) {}
    virtual void Rotate(float Deg, Vector3& Rot) {}
    virtual void Scale(Vector3& Scale) {}

    inline void SetCamera(Camera* NewCamPtr) { cam = NewCamPtr; }
    virtual void Look() {}

protected:
    Camera* cam;

};
