#pragma once
#include <vector>
#include <memory>
#include "maths.vector.hpp"
#include <Windows.h> // EVIL
#include <gl\GL.h>
#include <gl\GLU.h>
#include "texture.hpp"
#include "rotation.hpp"

class Object
{
public:
    Object() 
        : pos(0.0f, 0.0f, 0.0f)
        , rotation(0.0f, Vector3(0.0f, 1.0f, 0.0f))
        , scale(1.0f, 1.0f, 1.0f) 
    {};
    ~Object() 
    {};

    inline void setVerts(std::vector<Vector3>& newVerts)        { verts = newVerts; }    
    inline void setNormals(std::vector<Vector3>& newNormals)    { normals = newNormals; }    
    inline void setUVs(std::vector<Vector2>& newUVs)            { uvs = newUVs; }    
    inline void setColours(std::vector<Vector3>& newColours)    { colours = newColours; }
    inline void enableTexture()                                 { textureEnabled = true; }
    inline void disableTexture()                                { textureEnabled = false; }
    inline void setTexture(Texture& newTexture)                 { texture = newTexture; }    
    inline void setGLMode(GLuint newGLMode)                     { glMode = newGLMode; }
    inline void setPosition(Vector3& newPos)                    { pos = newPos; }    
    inline void setScale(Vector3& newScale)                     { scale = newScale; }
    inline void setRotation(Rotation& newRot)                   { rotation = newRot; }

    inline std::vector<Vector3>& getVerts()                     { return verts; }
    inline std::vector<Vector3>& getNormals()                   { return normals; }
    inline std::vector<Vector2>& getUVs()                       { return uvs; }
    inline std::vector<Vector3>& getColours()                   { return colours; }
    inline Texture& getTexture()                                { return texture; }
    inline Vector3 getPosition()                                { return pos; }
    inline Vector3 getScale()                                   { return scale; }    
    inline Rotation getRotation()                               { return rotation; }
    inline GLuint getGLMode()                                   { return glMode; }    
    inline bool UsingTexture()                                  { return textureEnabled; }

protected:

private:
    Vector3 pos;
    Vector3 scale;
    Rotation rotation;
    // More stuff
    std::vector<Vector3> verts;
    std::vector<Vector3> normals;    
    std::vector<Vector2> uvs;
    std::vector<Vector3> colours;
    bool textureEnabled;
    Texture texture;
    GLuint glMode;
};
using pObject = std::unique_ptr<Object>;