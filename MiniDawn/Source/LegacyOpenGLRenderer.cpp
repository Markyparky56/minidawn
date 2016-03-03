#include "LegacyOpenGLRenderer.hpp"

void LegacyOpenGLRenderer::Initialise()
{
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

// TODO: Update for vertex arrays
//void LegacyOpenGLRenderer::DrawObject(Object& Obj)
//{
//    if (Obj.UsingTexture())
//    {
//        Texture& tex = Obj.getTexture();
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, tex.getTexture());
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex.getUProperty());
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex.getVProperty());
//        glMatrixMode(GL_TEXTURE);
//        glPushMatrix();
//        Translate(tex.getOffset());
//        Rotate(tex.getRotation().deg, tex.getRotation().axis);
//        Scale(tex.getScale());
//    }
//    else
//    {
//        glBindTexture(GL_TEXTURE_2D, NULL);
//        glDisable(GL_TEXTURE_2D);
//    }
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix(); // Modify Geometry
//    Rotate(Obj.getRotation().deg, Obj.getRotation().axis);
//    Translate(Obj.getPosition());
//    Scale(Obj.getScale());
//    glBegin(Obj.getGLMode());
//
//    auto& verts = Obj.getVerts();
//    auto& normals = Obj.getNormals();
//    auto& uvs = Obj.getUVs();
//    auto& colours = Obj.getColours();
//    for (auto vert = verts.begin(); vert != verts.end(); vert++)
//    {
//        int place = std::distance(verts.begin(), vert);
//        Vector3& norm = normals[place];
//        Vector3& col = colours[place];
//        //Vector2& uv;  if (obj.UsingTexture()) { uv = uvs[place]; }
//        glNormal3f(norm.x, norm.y, norm.z);
//        glColor3f(col.x, col.y, col.z);
//        if (Obj.UsingTexture()) { glTexCoord2f(uvs[place].x, uvs[place].y); }
//        glVertex3f(vert->x, vert->y, vert->z);
//    }
//    glEnd();
//    glPopMatrix(); // Undo Geometry
//    glMatrixMode(GL_TEXTURE);
//    glPopMatrix(); // Undo Texture
//    glMatrixMode(GL_MODELVIEW); // Back to geometry just to be safe
//}

void LegacyOpenGLRenderer::DrawModel(Model& Model)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    if (Model.GetTexture().GetTexture())
    {
        Texture& tex = Model.GetTexture();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, Model.GetTexture().GetTexture());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        Vector3 &offset = tex.GetOffset(), &scale = tex.GetScale();
        Rotation &rot = tex.GetRotation();
        glTranslatef(offset.x, offset.y, offset.z);
        glRotatef(rot.deg, rot.axis.x, rot.axis.y, rot.axis.z);
        glScalef(scale.x, scale.y, scale.z);
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glVertexPointer(3, GL_FLOAT, 0, &(Model.GetVertices()[0]));
        glNormalPointer(GL_FLOAT, 0, &(Model.GetNormals()[0]));
        glTexCoordPointer(2, GL_FLOAT, 0, &(Model.GetTexCoords()[0]));
        glDrawArrays(GL_TRIANGLES, 0, Model.GetVertices().size() / 3);
        //glDrawElements(GL_TRIANGLES, Model.GetIndicies().size(), GL_UNSIGNED_INT, &(cube.GetIndicies()[0]));
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (Model.GetTexture().GetTexture())
    {
        glBindTexture(GL_TEXTURE_2D, NULL);
        glDisable(GL_TEXTURE_2D);
        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }
}

void LegacyOpenGLRenderer::Translate(Vector3 & Trans)
{
    glTranslatef(Trans.x, Trans.y, Trans.z);
}

void LegacyOpenGLRenderer::Rotate(float Deg, Vector3 & Rot)
{
    glRotatef(Deg, Rot.x, Rot.y, Rot.z);
}

void LegacyOpenGLRenderer::Scale(Vector3 & Scale)
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
