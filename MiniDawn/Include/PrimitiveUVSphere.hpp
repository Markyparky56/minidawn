#pragma once
#include "VertexObject.hpp"

using std::sinf;
using std::cosf;
using std::atan2;
using std::asin;

//class PrimitiveUVSphere : public VertexObject
//{
//public:
//    PrimitiveUVSphere(unsigned int InWidthDiv, unsigned int InHeightDiv)
//        : VertexObject(ObjectType::Primitive)
//    {
//        // (Based on the version found here: http://www.gamedev.net/topic/537269-procedural-sphere-creation/ by user ma_hty)
//        // Modified to use vectors, and to produce UVs (pending)
//
//        // Make sure we're working with clean vectors
//        vertices.clear();
//        normals.clear();
//        indicies.clear();
//        texCoords.clear();
//
//        // Make sure width and height aren't 0, 
//        // not sure what you'll get with 1 but it might be slightly less broken
//        int wDiv = (InWidthDiv == 0) ? 1 : InWidthDiv;
//        int hDiv = (InHeightDiv == 0) ? 1 : InHeightDiv;
//
//        // Allocate space for our vectors
//        int numVerts = (hDiv - 2) * wDiv + 2;
//        int numTris = (hDiv - 2) * (wDiv - 1) * 2;
//        vertices.reserve(numVerts);
//        normals.reserve(numVerts);
//        texCoords.reserve((numVerts / 3) * 2);
//        indicies.reserve(numTris);
//
//        // Generate Vertices & Normals
//        for (int y = 1; y < hDiv - 1; y++)
//        {
//            Vector2 prevUV(0.f, 0.f);
//            for (int x = 0; x < wDiv; x++)
//            {
//                float theta0, theta1, theta2;
//                theta0 = static_cast<float>(y) / (hDiv - 1) * PI;
//
//                float phi0, phi1, phi2;
//                phi0 = static_cast<float>(x) / (wDiv - 1) * PI2;
//
//                // Find point
//                Vector3 xyz0, xyz1, xyz2;
//                xyz0.x = sinf(theta0) * cosf(phi0);
//                xyz0.y = cosf(theta0);
//                xyz0.z = -sinf(theta0) * sinf(phi0);
//
//                // Save vertices
//                vertices.push_back(xyz0.x);
//                vertices.push_back(xyz0.y);
//                vertices.push_back(xyz0.z);
//
//                // Use vertices as normals, because it's a unit sphere
//                normals.push_back(xyz0.x);
//                normals.push_back(xyz0.y);
//                normals.push_back(xyz0.z);                
//            }
//        }
//        // Add poles
//        vertices.push_back(0.0f);
//        vertices.push_back(1.0f);
//        vertices.push_back(0.0f);
//
//        normals.push_back(0.0f);
//        normals.push_back(1.0f);
//        normals.push_back(0.0f);        
//
//        vertices.push_back(0.0f);
//        vertices.push_back(-1.0f);
//        vertices.push_back(0.0f);
//
//        normals.push_back(0.0f);
//        normals.push_back(-1.0f);
//        normals.push_back(0.0f);        
//
//        // Calculate Texture coordinates now that we have all our vertices
//        for (int i = 0; i < numVerts; i+=9)
//        {
//            // Magic maths from http://www.cse.msu.edu/~cse872/tutorial4.html (hallelujah!)
//            Vector2 uv1, uv2, uv3;
//            uv1.x = (std::atan2(vertices[i    ], vertices[i + 2]) / PI2) + 0.5f;
//            uv1.y = (std::asin (vertices[i + 1]) / PI) + 0.5f;
//            uv2.x = (std::atan2(vertices[i + 3], vertices[i + 5]) / PI2) + 0.5f;
//            uv2.y = (std::asin (vertices[i + 4]) / PI) + 0.5f;
//            uv3.x = (std::atan2(vertices[i + 6], vertices[i + 8]) / PI2) + 0.5f;
//            uv3.y = (std::asin (vertices[i + 7]) / PI) + 0.5f;
//
//            //float threshold = 0.7f;
//            //if (uv1.x > threshold || uv2.x > threshold || uv3.x > threshold)
//            //{
//            //    if (uv1.x < 1.0f - threshold)
//            //    {
//            //        uv1.x += 1.0f;
//            //    }
//            //    if (uv2.x < 1.0f - threshold)
//            //    {
//            //        uv2.x += 1.0f;
//            //    }
//            //    if (uv3.x < 1.0f - threshold)
//            //    {
//            //        uv3.x += 1.0f;
//            //    }
//            //}
//
//            texCoords.push_back(uv1.x);
//            texCoords.push_back(uv1.y);
//            texCoords.push_back(uv2.x);
//            texCoords.push_back(uv2.y);
//            texCoords.push_back(uv3.x);
//            texCoords.push_back(uv3.y);
//        }
//
//        texCoords.push_back(0.5f);
//        texCoords.push_back(1.0f);
//
//        texCoords.push_back(0.5f);
//        texCoords.push_back(0.0f);
//
//        // Calculate Indices
//        for (int y = 0; y < hDiv - 3; y++)
//        {
//            for (int x = 0; x < wDiv - 1; x++)
//            {
//                indicies.push_back((y  )*wDiv + x  );
//                indicies.push_back((y+1)*wDiv + x+1);
//                indicies.push_back((y  )*wDiv + x+1);
//                indicies.push_back((y  )*wDiv + x  );
//                indicies.push_back((y+1)*wDiv + x  );
//                indicies.push_back((y+1)*wDiv + x+1);
//            }
//            for (int x = 0; x < wDiv - 1; x++)
//            {
//                indicies.push_back((hDiv - 2)*wDiv);
//                indicies.push_back(x);
//                indicies.push_back(x + 1);
//                indicies.push_back((hDiv - 2)*wDiv + 1);
//                indicies.push_back((hDiv - 3)*wDiv + x + 1);
//                indicies.push_back((hDiv - 3)*wDiv + x);
//            }
//        }
//    }
//};

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

        float thetaStep = PI2 / static_cast<float>(InWidthDivisions);
        float deltaStep = PI / static_cast<float>(InHeightDivisions);

        // Allocate space for vectors
        int numVerts = ((InWidthDivisions * InHeightDivisions) * 6) + 2;
        int numTris = numVerts / 3;
        int numTexCoords = (numVerts / 3) * 2;
        
        vertices.reserve(numVerts);
        normals.reserve(numVerts);
        texCoords.reserve(numTexCoords);
        indicies.reserve(numTris); // Think we'll pass on indices though

        for (int y = 0; y < InHeightDivisions; y++)
        {
            for (int x = 0; x < InWidthDivisions; x++)
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
                    Vector2 uv(0.5f + atan2(p.z, -p.x) / PI2, 0.5 - asin(p.y) / PI);
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
