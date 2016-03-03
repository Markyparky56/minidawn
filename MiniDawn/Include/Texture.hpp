#pragma once
#include <memory>
#include "maths.vector.hpp"
#include "rotation.hpp"
#include <gl\GL.h>
#include <gl\GLU.h>

class Texture
{
private:
    GLuint texture;
    GLuint uProp;
    GLuint vProp;

    Vector3 offset;
    Rotation rotation;
    Vector3 scale;

public:
    Texture() : Texture(NULL) {}
    Texture(GLuint tex) 
        : texture(tex)
        , uProp(NULL)
        , vProp(NULL)
        , offset(0.0f, 0.0f, 0.0f)
        , rotation(0.0f, Vector3(0.0f, 1.0f, 0.0f))
        , scale(1.0f, 1.0f, 1.0f) 
    {}
    ~Texture() {}

    inline void SetTexture(GLuint tex)      { texture = tex; }
    inline void SetRotation(Rotation& rot)  { rotation = rot; }
    inline void SetUProperty(GLuint prop)   { uProp = prop; }
    inline void SetOffset(Vector3& off)     { offset = off; }
    inline void SetVProperty(GLuint prop)   { vProp = prop; }
    inline void SetScale(Vector3& scal)     { scale = scal; }

    inline Vector3 GetOffset()              { return offset; }
    inline GLuint GetTexture()              { return texture; }
    inline GLuint GetVProperty()            { return vProp; }
    inline GLuint GetUProperty()            { return uProp; }
    inline Rotation GetRotation()           { return rotation; }
    inline Vector3 GetScale()               { return scale; }

};
using pTexture = std::unique_ptr<Texture>;
