#include "LegacyOpenGLRenderer.hpp"
#include "dbout.hpp"
#include <stack>

void LegacyOpenGLRenderer::Initialise()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending transparency
}

void LegacyOpenGLRenderer::Shutdown()
{
}

// Ignore
void LegacyOpenGLRenderer::StartDrawing(GLenum GLType)
{
    glBegin(GLType);
}

// Ignore
void LegacyOpenGLRenderer::EndDrawing()
{
    glEnd();
}

void LegacyOpenGLRenderer::DrawModel(const Model& InModel)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    if (InModel.GetTexture().GetTexture())
    {
        const Texture& tex = InModel.GetTexture();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, InModel.GetTexture().GetTexture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        const Vector3 &offset = tex.GetOffset(), &scale = tex.GetScale();
        const Rotation &rot = tex.GetRotation();
        glTranslatef(offset.x, offset.y, offset.z);
        glRotatef(rot.deg, rot.axis.x, rot.axis.y, rot.axis.z);
        glScalef(scale.x, scale.y, scale.z);
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glVertexPointer(3, GL_FLOAT, 0, &(InModel.GetVertices()[0]));
        glNormalPointer(GL_FLOAT, 0, &(InModel.GetNormals()[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(InModel.GetTexCoords()[0]));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(InModel.GetVertices().size()) / 3);
        //glDrawElements(GL_TRIANGLES, Model.GetIndicies().size(), GL_UNSIGNED_INT, &(cube.GetIndicies()[0]));
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (InModel.GetTexture().GetTexture())
    {
        glBindTexture(GL_TEXTURE_2D, NULL);
        glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
}

void LegacyOpenGLRenderer::DrawSkybox(const Skybox & InSkybox)
{
    // Turn Depth off
    glDisable(GL_DEPTH_TEST);
    const VertexObject* vo = InSkybox.GetCube();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    const Texture& tex = vo->GetTexture();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, vo->GetTexture().GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glMatrixMode(GL_TEXTURE);
    glPushMatrix(); // Texture matrix manipulation
    const Vector3 &offset = tex.GetOffset(), &scale = tex.GetScale();
    const Rotation &rot = tex.GetRotation();
    glTranslatef(offset.x, offset.y, offset.z);
    glRotatef(rot.deg, rot.axis.x, rot.axis.y, rot.axis.z);
    glScalef(scale.x, scale.y, scale.z);
    glMatrixMode(GL_MODELVIEW);

    // Translate to the camera position
    glPushMatrix();
    Translate(cam->GetPosition());

    // Setup Arrays
    glVertexPointer(3, GL_FLOAT, 0, &(vo->GetVertices()[0]));
    glNormalPointer(GL_FLOAT, 0, &(vo->GetNormals()[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(vo->GetTexCoords()[0]));

    // Draw!
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vo->GetIndicies().size()), GL_UNSIGNED_INT, &(vo->GetIndicies()[0]));

    glPopMatrix(); 

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindTexture(GL_TEXTURE_2D, NULL);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_TEXTURE);
    glPopMatrix(); // Undo Texture matrix manipulation
    glMatrixMode(GL_MODELVIEW);
    // Turn Depth test on again
    glEnable(GL_DEPTH_TEST);
}

void LegacyOpenGLRenderer::DrawSkysphere(const Skysphere & InSkysphere)
{
    // Turn Depth off
    glDisable(GL_DEPTH_TEST);
    const VertexObject* vo = InSkysphere.GetSphere();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    const Texture& tex = vo->GetTexture();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, vo->GetTexture().GetTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glMatrixMode(GL_TEXTURE);
    glPushMatrix(); // Texture matrix manipulation
    const Vector3 &offset = tex.GetOffset(), &scale = tex.GetScale();
    const Rotation &rot = tex.GetRotation();
    glTranslatef(offset.x, offset.y, offset.z);
    glRotatef(rot.deg, rot.axis.x, rot.axis.y, rot.axis.z);
    glScalef(scale.x, scale.y, scale.z);
    glMatrixMode(GL_MODELVIEW);

    // Translate to the camera position
    glPushMatrix();
    Translate(cam->GetPosition());

    // Setup Arrays
    glVertexPointer(3, GL_FLOAT, 0, &(vo->GetVertices()[0]));
    glNormalPointer(GL_FLOAT, 0, &(vo->GetNormals()[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(vo->GetTexCoords()[0]));

    // Draw!
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vo->GetVertices().size()) / 3);

    glPopMatrix();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBindTexture(GL_TEXTURE_2D, NULL);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_TEXTURE);
    glPopMatrix(); // Undo Texture matrix manipulation
    glMatrixMode(GL_MODELVIEW);
    // Turn Depth test on again
    glEnable(GL_DEPTH_TEST);
}

void LegacyOpenGLRenderer::DrawObject(const Object& InObject)
{
    const VertexObject* vo = InObject.GetVO().Get();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (InObject.UsingTransparency())
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending transparency
    }

    if (InObject.UsingTexture())
    {
        const Texture& tex = vo->GetTexture();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, vo->GetTexture().GetTexture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        const Vector3 &offset = tex.GetOffset(), &scale = tex.GetScale();
        const Rotation &rot = tex.GetRotation();
        glTranslatef(offset.x, offset.y, offset.z);
        glRotatef(rot.deg, rot.axis.x, rot.axis.y, rot.axis.z);
        glScalef(scale.x, scale.y, scale.z);
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // Modify Geometry
    if (InObject.GetParent() != nullptr)
    {
        std::stack<const Transform*> transforms;
        // Fill transforms with each inherited transform
        const Object *parent = InObject.GetParent();
        const Object *prevParent = nullptr;
        while (parent != nullptr)
        {
            transforms.push(&(parent->GetTransform()));
            prevParent = parent;
            parent = parent->GetParent();
        }
        // Apply transforms
        while (!transforms.empty())
        {
            if (prevParent->GetTransform().InheritPreRotation()) Rotate(transforms.top()->GetPreRotation().deg, transforms.top()->GetPreRotation().axis);
            if (prevParent->GetTransform().InheritPosition())    Translate(transforms.top()->GetPosition());
            if (prevParent->GetTransform().InheritRotation())    Rotate(transforms.top()->GetRotation().deg, transforms.top()->GetRotation().axis);
            if (prevParent->GetTransform().InheritScale())       Scale(transforms.top()->GetScale());
            transforms.pop();
        }
    }
    // Apply our own transform
    Rotate(InObject.GetTransform().GetPreRotation().deg, InObject.GetTransform().GetPreRotation().axis);
    Translate(InObject.GetTransform().GetPosition());
    Rotate(InObject.GetTransform().GetRotation().deg, InObject.GetTransform().GetRotation().axis);
    Scale(InObject.GetTransform().GetScale());
    
    // Setup Arrays
    glVertexPointer(3, GL_FLOAT, 0, &(vo->GetVertices()[0]));
    glNormalPointer(GL_FLOAT, 0, &(vo->GetNormals()[0]));
    glTexCoordPointer(2, GL_FLOAT, 0, &(vo->GetTexCoords()[0]));

    switch (vo->GetType())
    {
    case ObjectType::Model:
    case ObjectType::PrimitiveUnIndexed:
        glDrawArrays(InObject.GetGLMode(), 0, static_cast<GLsizei>(vo->GetVertices().size()) / 3);
        break;
    case ObjectType::PrimitiveIndexed:
        glDrawElements(InObject.GetGLMode(), static_cast<GLsizei>(vo->GetIndicies().size()), GL_UNSIGNED_INT, &(vo->GetIndicies()[0]));
        break;
    default:
        // Unreachable code!
        break;
    }
    
    glPopMatrix(); // Undo geometry modifications

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (InObject.UsingTransparency())
    {
        glDisable(GL_BLEND);
    }

    if (InObject.UsingTexture())
    {
        glBindTexture(GL_TEXTURE_2D, NULL);
        glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
}

void LegacyOpenGLRenderer::Translate(const Vector3 & Trans)
{
    glTranslatef(Trans.x, Trans.y, Trans.z);
}

void LegacyOpenGLRenderer::Rotate(const float Deg, const Vector3 & Rot)
{
    glRotatef(Deg, Rot.x, Rot.y, Rot.z);
}

void LegacyOpenGLRenderer::Scale(const Vector3 & Scale)
{
    glScalef(Scale.x, Scale.y, Scale.z);
}

void LegacyOpenGLRenderer::Look()
{
    Vector3& camPos  = cam->GetPosition();
    Vector3& camLook = cam->GetLookAt();
    Vector3& camUp   = cam->GetUp();

    gluLookAt(camPos.x,     camPos.y,   camPos.z,
              camLook.x,    camLook.y,  camLook.z,
              camUp.x,      camUp.y,    camUp.z);
}

void LegacyOpenGLRenderer::SetColour(Vector3 & Col)
{
    glColor3f(Col.x, Col.y, Col.z);
}

void LegacyOpenGLRenderer::EnableLight(GLenum Light)
{
    glEnable(Light);
}

void LegacyOpenGLRenderer::SetLightProperty(GLenum Light, GLenum LightProperty, GLfloat PropertyArray[])
{
    glLightfv(Light, LightProperty, PropertyArray);
}

void LegacyOpenGLRenderer::SetLightProperty(GLenum Light, GLenum LightProperty, GLfloat Prop)
{
    glLightf(Light, LightProperty, Prop);
}

void LegacyOpenGLRenderer::SetMaterial(GLenum Face, GLenum Property, GLfloat Properties[])
{
    glMaterialfv(Face, Property, Properties);
}

GLuint LegacyOpenGLRenderer::LoadTexture(char * Loc, int LoadType, int IdType, int Flags)
{
    GLuint img;
    img = SOIL_load_OGL_texture(Loc, LoadType, IdType, Flags);
    return img;
}
