#pragma once
#include "VertexObject.hpp"

// Define a Primitive Cube Object
class PrimitiveCube : public VertexObject
{
public:
    PrimitiveCube()
        : VertexObject(ObjectType::Primitive)
    {
        vertices = { {
                // Front Face
                -0.5f, -0.5f, 0.5f, // #0
                 0.5f, -0.5f, 0.5f, // #1
                 0.5f,  0.5f, 0.5f, // #2
                -0.5f,  0.5f, 0.5f, // #3
                // Back Face
                -0.5f, -0.5f, -0.5f, // #4
                 0.5f, -0.5f, -0.5f, // #5
                 0.5f,  0.5f, -0.5f, // #6
                -0.5f,  0.5f, -0.5f, // #7
                // Top Face
                -0.5f, 0.5f,  0.5f, // #8
                 0.5f, 0.5f,  0.5f, // #9
                 0.5f, 0.5f, -0.5f, // #10
                -0.5f, 0.5f, -0.5f, // #11
                // Bottom Face
                -0.5f, -0.5f,  0.5f, // #12
                 0.5f, -0.5f,  0.5f, // #13
                 0.5f, -0.5f, -0.5f, // #14
                -0.5f, -0.5f, -0.5f, // #15
                // Left Face
                -0.5f, -0.5f,  0.5f, // #16
                -0.5f,  0.5f,  0.5f, // #17
                -0.5f,  0.5f, -0.5f, // #18
                -0.5f, -0.5f, -0.5f, // #19
                // Right Face
                0.5f, -0.5f,  0.5f, // #20
                0.5f,  0.5f,  0.5f, // #21
                0.5f,  0.5f, -0.5f, // #22
                0.5f, -0.5f, -0.5f  // #23
            } };
        normals = { {
                0.0f, 0.0f, 1.0f, // Front Normals
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,
                
                0.0f, 0.0f, -1.0f, // Back Normals
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,

                0.0f, 1.0f, 0.0f, // Top Normals
                0.0f, 1.0f, 0.0f, 
                0.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                
                0.0f, -1.0f, 0.0f, // Bottom Normals
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,
                0.0f, -1.0f, 0.0f,

                -1.0f, 0.0f, 0.0f, // Left Normals
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,
                -1.0f, 0.0f, 0.0f,

                1.0f, 0.0f, 0.0f, // Right Normals
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f
            } };
        texCoords = { {
                0.0f, 0.0f, // Front UVs
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,

                1.0f, 0.0f, // Back UVs
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,

                0.0f, 1.0f, // Top UVs
                1.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,
                
                1.0f, 1.0f, // Bottom UVs
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                1.0f, 0.0f, // Left UVs
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,

                0.0f, 0.0f,
                0.0f, 1.0f, // Right UVs
                1.0f, 1.0f,
                1.0f, 0.0f
            } };
        indicies = { {
                0, 1, 3, // Front Face
                1, 2, 3,
                
                5, 4, 6, // Back Face
                4, 7, 6,
                
                8, 9, 11, // Top Face
                9, 10, 11,
                
                13, 12, 14, // Bottom Face
                12, 15, 14,
                
                16, 17, 19, // Left Face
                17, 18, 19,
                
                21, 20, 22, // Right Face
                20, 23, 22
            } };
    }
};
