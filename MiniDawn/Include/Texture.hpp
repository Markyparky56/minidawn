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

    inline void setTexture(GLuint tex)      { texture = tex; }
    inline void setRotation(Rotation& rot)  { rotation = rot; }
    inline void setUProperty(GLuint prop)   { uProp = prop; }
    inline void setOffset(Vector3& off)     { offset = off; }
    inline void setVProperty(GLuint prop)   { vProp = prop; }
    inline void setScale(Vector3& scal)     { scale = scal; }

    inline Vector3 getOffset()              { return offset; }
    inline GLuint getTexture()              { return texture; }
    inline GLuint getVProperty()            { return vProp; }
    inline GLuint getUProperty()            { return uProp; }
    inline Rotation getRotation()           { return rotation; }
    inline Vector3 getScale()               { return scale; }

};
using pTexture = std::unique_ptr<Texture>;
