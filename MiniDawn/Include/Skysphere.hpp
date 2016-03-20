#pragma once
#include "Object.hpp"
#include "PrimitiveUVSphere.hpp"
#include "UniqueObj.hpp"

class Skysphere
{
public:
    Skysphere(unsigned int InWidthDivisions = 32U, unsigned int InHeightDivisions = 16U)
    {
        sphere = MakeUnique<PrimitiveUVSphere>(InWidthDivisions, InHeightDivisions);
    }

    inline void SetSkysphereTexture(Texture& NewTex) { sphere->GetTexture() = NewTex; }
    inline const VertexObject* GetSphere() const { return sphere.Get(); }
    inline       VertexObject* GetSphere()       { return sphere.Get(); }

private:
    UniquePtr<VertexObject> sphere;
};