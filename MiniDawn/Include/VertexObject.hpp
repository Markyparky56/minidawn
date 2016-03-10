#pragma once
// Primitive Object Base Class
#include "maths.vector.hpp"
#include <vector>
#include <Windows.h> // I cri ery tim
#include <gl/GL.h>
#include "Texture.hpp"

enum class ObjectType { Primitive, Model };

class VertexObject
{
public:
    VertexObject(ObjectType InType)
        : type(InType)
    {}
    inline const std::vector<float>& GetVertices() const    { return vertices; }
    inline       std::vector<float>& GetVertices()          { return vertices; }
    inline const std::vector<float>& GetNormals() const     { return normals; }
    inline       std::vector<float>& GetNormals()           { return normals; }
    inline const std::vector<float>& GetTexCoords() const   { return texCoords; }
    inline       std::vector<float>& GetTexCoords()         { return texCoords; }
    inline const std::vector<GLuint>& GetIndicies() const   { return indicies; }
    inline       std::vector<GLuint>& GetIndicies()         { return indicies; }
    inline const Texture& GetTexture() const                { return texture; }
    inline       Texture& GetTexture()                      { return texture; }
    inline const ObjectType GetType() const                 { return type; }

protected:
    std::vector<float> vertices, normals, texCoords;
    std::vector<GLuint> indicies;
    Texture texture; // Could be a pointer

    ObjectType type;
};
