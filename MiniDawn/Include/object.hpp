#pragma once
#include "Model.h"
#include <memory>

// Basically an extension to the model class which stores position data
class Object
{
public:
    Object()
        : pos(0.0f, 0.0f, 0.0f)
        , rotation(0.0f, Vector3(0.0f, 1.0f, 0.0f))
        , scale(1.0f, 1.0f, 1.0f)
        , glMode(GL_TRIANGLES)
        , textureEnabled(false)
        , model(nullptr)
    {}
    ~Object()
    {}

    inline void EnableTexture()       { textureEnabled = true; }
    inline void DisableTexture()      { textureEnabled = false; }
    
    inline void SetPosition(Vector3& NewPos)    { pos = NewPos; }
    inline void SetScale(Vector3& NewScale)     { scale = NewScale; }
    inline void SetRotation(Rotation& NewRot)   { rotation = NewRot; }
    inline void SetGLMode(GLuint NewGLMode)     { glMode = NewGLMode; }
    inline void SetVO(VertexObject* NewModel)       { model = NewModel; }

    inline const Vector3&  GetPosition() const  { return pos; }
    inline const Vector3&  GetScale() const     { return scale; }
    inline const Rotation& GetRotation() const  { return rotation; }
    inline const GLuint   GetGLMode() const     { return glMode; }
    inline VertexObject* GetVO() const        { return model; };
    inline bool  UsingTexture() const        { return textureEnabled; }

private:
    VertexObject* model; // Use a pointer so we can share models between objects

    Vector3 pos, scale;
    Rotation rotation;

    bool textureEnabled;
    GLuint glMode; // If you're weird and want to use quads
};
using pObject = std::unique_ptr<Object>;
using spObject = std::shared_ptr<Object>;
