#pragma once
#include "VertexObject.hpp"

using std::sinf;
using std::cosf;

class PrimitiveDisc : public VertexObject
{
    const double PI = 3.1415926535897932384626433832795;
    const double PI2 = (PI * 2);

public:
    enum class GLMode { Triangles, TriangleFan };
    PrimitiveDisc(unsigned int segments, GLMode mode = GLMode::TriangleFan)
        : VertexObject(ObjectType::PrimitiveUnIndexed)
    {
        switch (mode)
        {
        case GLMode::TriangleFan:
            GenerateTriangleFan(segments);
            break;

        case GLMode::Triangles:
            GenerateTriangles(segments);
            break;
        }
    }

    inline void GenerateTriangleFan(unsigned int segments)
    {
        vertices.clear();
        normals.clear();
        indicies.clear();
        texCoords.clear();

        float thetaStep = static_cast<float>(PI2 / static_cast<double>(segments));

        int numVerts = segments + 2; // Edge points + center + return to start
        int numTexCoords = (numVerts / 3) * 2;

        vertices.reserve(numVerts);
        normals.reserve(numVerts);
        texCoords.reserve(numVerts);

        // Store start center of the disc
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        texCoords.push_back(0.5f);
        texCoords.push_back(0.5f);

        float theta = 0.0f;
        for (int i = 0; i < segments + 1; i++)
        {
            // Find point on edge
            Vector3 xyz(sinf(theta), 0.0f, cosf(theta));
            
            // Calculate vertex on edge
            vertices.push_back(xyz.x);
            vertices.push_back(0.0f);
            vertices.push_back(xyz.z);

            // Add Normal
            normals.push_back(0.0f);
            normals.push_back(1.0f);
            normals.push_back(0.0f);

            // Work out texture coordinate
            Vector2 uv((xyz.x * 0.5f) + 0.5f, (xyz.z * 0.5f) + 0.5f);
            texCoords.push_back(uv.x);
            texCoords.push_back(uv.y);
            if(i < segments)
            {
                theta += thetaStep;
            }
            else
            {
                theta = 0.0f; // Clamp the final value back to the beginning
            }
        }
    }

    inline void GenerateTriangles(unsigned int segments)
    {
        vertices.clear();
        normals.clear();
        indicies.clear();
        texCoords.clear();

        float thetaStep = PI2 / static_cast<float>(segments);

        int numVerts = (segments) * 3; // Edge points + center + return to start
        int numTexCoords = (numVerts / 3) * 2;

        vertices.reserve(numVerts);
        normals.reserve(numVerts);
        texCoords.reserve(numVerts);

        float theta1 = 0.0f, theta2 = thetaStep;
        for (int i = 0; i < segments + 1; i++)
        {
            // Find the two points for this segment
            Vector3 xyz1, xyz2;
            xyz1 = Vector3(sinf(theta1), 0.0f, cosf(theta1));
            xyz2 = Vector3(sinf(theta2), 0.0f, cosf(theta2));

            Vector2 uv1, uv2;
            uv1 = Vector2((xyz1.x * 0.5f) + 0.5f, (xyz1.z * 0.5f) + 0.5f);
            uv2 = Vector2((xyz2.x * 0.5f) + 0.5f, (xyz2.z * 0.5f) + 0.5f);

            // Push the vertices, normals and texture coordinates
            // Center of disc
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);

            vertices.push_back(xyz1.x);
            vertices.push_back(0.0f);
            vertices.push_back(xyz1.z);

            vertices.push_back(xyz2.x);
            vertices.push_back(0.0f);
            vertices.push_back(xyz2.z);

            for (int i = 0; i < 3; i++)
            {
                normals.push_back(0.0f);
                normals.push_back(1.0f);
                normals.push_back(0.0f);
            }

            texCoords.push_back(0.5f);
            texCoords.push_back(0.5f);

            texCoords.push_back(uv1.x);
            texCoords.push_back(uv1.y);
            
            texCoords.push_back(uv2.x);
            texCoords.push_back(uv2.y);

            if (i < segments)
            {
                theta1 += thetaStep;
                theta2 += thetaStep;
            }
            else
            {
                theta1 += thetaStep;
                theta2 = 0.0f; // Clamp back to the beginning
            }
        }
    }
};