#pragma once
#include "Object.hpp"
#include "PrimitiveCube.hpp"
#include "UniqueObj.hpp"

class Skybox
{
public:
    Skybox()
    {
        cube = MakeUnique<PrimitiveCube>();
        // Override the standard Primitive Cube's Texture Coordinates
        cube->GetTexCoords() = std::vector<float>{{
                0.5f , 0.5f,
                0.25f, 0.5f,
                0.25f, 0.25f, // Front UVs
                0.5f , 0.25f,                

                0.75f, 0.5f,
                1.0f , 0.5f,
                1.0f , 0.25f, // Back UVs
                0.75f, 0.25f,                

                0.5f, 0.25f, // Top UVs
                0.25f , 0.25f,
                0.25f , 0.0f,
                0.5f, 0.0f,
                
                0.5f , 0.5f, // Bottom UVs
                0.25f, 0.5f,
                0.25f, 0.75f,
                0.5f , 0.75f,

                0.5f, 0.5f,
                0.5f, 0.25f,
                0.75f , 0.25f, // Left UVs
                0.75f , 0.5f,
            
                0.25f , 0.5f,
                0.25f , 0.25f,
                0.0f, 0.25f, // Right UVs                
                0.0f, 0.5f               
            }};
    }

    inline void SetSkyboxTexture(Texture& NewTex) { cube->GetTexture() = NewTex; }
    inline const VertexObject* GetCube() const { return cube.Get(); }
    inline       VertexObject* GetCube()       { return cube.Get(); }

private:
    UniquePtr<VertexObject> cube;
};