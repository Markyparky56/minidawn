#pragma once
#include "VertexObject.hpp"

using std::sinf;
using std::cosf;
using std::atan2;
using std::asin;

class PrimitiveUVSphere : public VertexObject
{
    const double PI = 3.1415926535897932384626433832795;
    const double PI2 = (PI * 2);

public:
    PrimitiveUVSphere(unsigned int InWidthDivisions, unsigned int InHeightDivisions)
        : VertexObject(ObjectType::PrimitiveUnIndexed)
    {
        vertices.clear();
        normals.clear();
        indicies.clear();
        texCoords.clear();

        float thetaStep = static_cast<float>(PI2 / static_cast<double>(InWidthDivisions));
        float deltaStep = static_cast<float>(PI / static_cast<double>(InHeightDivisions));

        // Allocate space for vectors
        int numVerts = ((InWidthDivisions * InHeightDivisions) * 6); 
        int numTris = numVerts / 3;
        int numTexCoords = (numVerts / 3) * 2;
        
        vertices.reserve(numVerts);
        normals.reserve(numVerts);
        texCoords.reserve(numTexCoords);
        indicies.reserve(numTris); // Think we'll pass on indices though

        for (unsigned int y = 0; y < InHeightDivisions; y++)
        {
            for (unsigned int x = 0; x < InWidthDivisions; x++)
            {
                float theta = static_cast<float>(y) * thetaStep;
                float delta = static_cast<float>(x) * deltaStep;

                // Calculate the 6 vertices for this step
                std::vector<Vector3> points =
                {
                    /*1*/Vector3(sinf(theta + thetaStep)*cosf(delta), cosf(theta + thetaStep), -sinf(theta + thetaStep)*sinf(delta)),
                    /*2*/Vector3(sinf(theta)*cosf(delta + deltaStep), cosf(theta), -sinf(theta)*sinf(delta + deltaStep)),
                    /*3*/Vector3(sinf(theta)*cosf(delta), cosf(theta), -sinf(theta)*sinf(delta)),
                    /*4*/Vector3(sinf(theta + thetaStep)*cosf(delta + deltaStep), cosf(theta + thetaStep), -sinf(theta + thetaStep)*sinf(delta + deltaStep)),
                    /*5*/Vector3(sinf(theta)*cosf(delta + deltaStep), cosf(theta), -sinf(theta)*sinf(delta + deltaStep)),
                    /*6*/Vector3(sinf(theta + thetaStep)*cosf(delta), cosf(theta + thetaStep), -sinf(theta + thetaStep)*sinf(delta)),
                };

                // Calculate the 6 texture coordinates for this step
                std::vector<Vector2> uvs;
                for(auto& p : points)
                {
                    Vector2 uv(0.5f + atan2f(p.z, - p.x) / static_cast<float>(PI2), 0.5f - asinf(p.y) / static_cast<float>(PI));
                    uvs.push_back(uv);
                }

                // Adjust them if we cross the seam between 1.0 and 0
                float threshold = 0.9f;
                for (int i = 0; i < 2; i++)
                {
                    if (uvs[0 + i*3].x > threshold || uvs[1 + i*3].x > threshold || uvs[2 + i*3].x > threshold)
                    {
                        if (uvs[0 + i * 3].x < 1.0f - threshold)
                        {
                            uvs[0 + i * 3].x += 1.0f;
                        }
                        if (uvs[1 + i * 3].x < 1.0f - threshold)
                        {
                            uvs[1 + i * 3].x += 1.0f;
                        }
                        if (uvs[2 + i * 3].x < 1.0f - threshold)
                        {
                            uvs[2 + i * 3].x += 1.0f;
                        }
                    }
                }

                // Push the new points onto the vector lists                
                for (auto& p : points)
                {
                    vertices.push_back(p.x);
                    vertices.push_back(p.y);
                    vertices.push_back(p.z);
                }
                for (auto& n : points)
                {
                    normals.push_back(n.x);
                    normals.push_back(n.y);
                    normals.push_back(n.z);
                }
                for (auto& uv : uvs)
                {
                    texCoords.push_back(uv.x);
                    texCoords.push_back(uv.y);
                }
            }
        }
    }
};
