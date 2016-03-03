#pragma once
// Primitive Object Base Class
#include "maths.vector.hpp"
#include <vector>
#include <gl/GL.h>

class PrimitiveBase
{
public:
    inline std::vector<float>& GetVertices()    { return vertices; }
    inline std::vector<float>& GetNormals()     { return normals; }
    inline std::vector<float>& GetTexCoords()   { return texCoords; }
    inline std::vector<GLuint>& GetIndicies()    { return indicies; }

protected:
    std::vector<float> vertices, normals, texCoords;
    std::vector<GLuint> indicies;
};
