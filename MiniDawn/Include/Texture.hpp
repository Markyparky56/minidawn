#pragma once
#include "UniquePtr.hpp"
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

    inline const Vector3 GetOffset() const      { return offset; }
    inline const GLuint GetTexture() const      { return texture; }
    inline const GLuint GetVProperty() const    { return vProp; }
    inline const GLuint GetUProperty() const    { return uProp; }
    inline const Rotation GetRotation() const   { return rotation; }
    inline const Vector3 GetScale() const       { return scale; }

};
using pTexture = UniquePtr<Texture>;
