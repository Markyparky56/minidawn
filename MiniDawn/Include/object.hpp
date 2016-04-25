#pragma once
#include "Model.h"
#include "UniquePtr.hpp"
#include "SharedRef.hpp"
#include "Transform.hpp"
#include <string>

// Basically an extension to the model class which stores position data
class Object
{
public:
    Object()
        : glMode(GL_TRIANGLES)
        , textureEnabled(false)
        , transparencyEnabled(false)
        //, model(nullptr)
        , parent(nullptr)
    {}
    ~Object()
    {}

    inline void EnableTexture()       { textureEnabled = true; }
    inline void DisableTexture()      { textureEnabled = false; }
    inline void EnableTransparency()  { transparencyEnabled = true; }
    inline void DisableTransparency() { transparencyEnabled = false; }
    
    //inline void SetPosition(Vector3& NewPos)    { pos = NewPos; }
    //inline void SetScale(Vector3& NewScale)     { scale = NewScale; }
    //inline void SetRotation(Rotation& NewRot)   { rotation = NewRot; }
    
    inline void SetTransform(Transform& NewTransform)   { transform = NewTransform; }
    inline void SetGLMode(GLuint NewGLMode)             { glMode = NewGLMode; }
    inline void SetVO(SharedPtr<VertexObject> NewModel) { model = NewModel; }
    inline void SetName(std::wstring NewName)           { name = NewName; }
    inline void SetParent(const Object *NewParent)      { parent = NewParent; }
    inline void SetParent(const Object *NewParent, 
                          TransformInheritance& inheritanceSettings) 
    { 
        parent = NewParent; 
        transform.transformInheritance = inheritanceSettings;
    }
    inline void SetParentInheritance(TransformInheritance& inheritanceSettings) { transform.transformInheritance = inheritanceSettings; }

    //inline const Vector3& GetPosition()  const  { return pos; }
    //inline       Vector3& GetPosition()         { return pos; }
    //inline const Vector3& GetScale()     const  { return scale; }
    //inline       Vector3& GetScale()            { return scale; }
    //inline const Rotation& GetRotation() const  { return rotation; }
    //inline       Rotation& GetRotation()        { return rotation; }
    
    inline const Transform& GetTransform() const { return transform; }
    inline       Transform& GetTransform()       { return transform; }
    inline const GLuint GetGLMode()        const { return glMode; }
    inline       GLuint GetGLMode()              { return glMode; }
    inline const Object *GetParent()             { return parent; }
    inline const Object *GetParent()       const { return parent; }    
    inline bool  UsingTexture()            const { return textureEnabled; }
    inline bool  UsingTransparency()       const { return transparencyEnabled; }
    inline const std::wstring& GetName()   const { return name; }
    inline       std::wstring& GetName()         { return name; }
    inline       SharedPtr<VertexObject> GetVO()           { return model; }
    inline const SharedPtr<VertexObject> GetVO()     const { return model; }
private:
    SharedPtr<VertexObject> model; // Use a pointer so we can share models between objects
    const Object *parent;
    
    //Vector3 pos, scale;
    //Rotation rotation;
    
    Transform transform;
    std::wstring name;

    bool textureEnabled;
    bool transparencyEnabled;
    GLuint glMode; // If you're weird and want to use quads
};
using pObject = UniquePtr<Object>;
using spObject = SharedPtr<Object>;
