#pragma once
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Renderer.hpp"

class LegacyOpenGLRenderer : public Renderer
{
public:
    LegacyOpenGLRenderer() 
    {}
    ~LegacyOpenGLRenderer()
    {}

    void Initialise() override;
    void Shutdown() override;

    void StartDrawing(GLenum GLType);
    void EndDrawing() override;

    void DrawObject(Object& Obj) override;
    void DrawModel(Model& Model) override;

    void Translate(Vector3& Trans) override;
    void Rotate(float Deg, Vector3& Rot) override;
    void Scale(Vector3& Scale) override;

    void Look() override;

    // LEgacy function wrappers
    void SetColour(Vector3& Col);
    void EnableLight(GLenum Light);
    void SetLightProperty(GLenum Light, GLenum LightProperty, GLfloat PropertyArray[]);
    void SetLightProperty(GLenum Light, GLenum LightProperty, GLfloat Prop);
    void SetMaterial(GLenum face, GLenum Property, GLfloat Properties[]);
    GLuint LoadTexture(char* Loc, int LoadType, int IdType, int Flags);

private:

};
