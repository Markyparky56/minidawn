#include "TestScene.hpp"
#include "GMiniDawnEngine.hpp"
#ifdef _DEBUG
#include "dbout.hpp"
#endif

// Create Objects
void TestScene::Setup()
{
    windowDef = parentApp->GetActiveWindow()->GetDef();
    screenCentre = Vector2(windowDef->Width / 2, windowDef->Height / 2);

    Object* cube = new Object;
    SetupCube36(cube);
    objects.push_back(pObject(cube));

    newPos = Vector2(0.0f, 0.0f);
    camRot.pitch = 0.0f;
    camRot.yaw = 0.0f;
    camRot.roll = 0.0f;

    camera = std::make_unique<Camera>();
    camera->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    camera->SetUp(Vector3(0.0f, 1.0f, 0.0f));
    camera->Update();
    renderer->SetCamera(camera.get());
    
    model.Load("models/teapot.obj", "sphericalheightmap.png");
}

// Logic
void TestScene::Update()
{
    Rotation oldRot = objects[0]->getRotation();
    oldRot.deg += 15 * deltaTime;
    objects[0]->setRotation(oldRot);

    // Handle input
    if (inputSystem->IsKeyDown('P'))
    {
        wireframeMode = !wireframeMode;
    }
    
    if (inputSystem->IsKeyDown(VK_ESCAPE)) // Hit escape to exit, so if we decide to lock the mouse we can still quit
    {
        GMiniDawnEngine.RequestEnd();
    }
    newPos.x = inputSystem->GetMouseX();
    newPos.y = inputSystem->GetMouseY();

    // Reset the mouse position
    POINT pt;
    pt.x = screenCentre.x;
    pt.y = screenCentre.y;
    ClientToScreen(static_cast<HWND>(parentApp->GetActiveWindow()->getHwnd()), &pt);
    SetCursorPos(pt.x, pt.y);

    // Convert mouse movement to camera rotation
    Vector2 dir = screenCentre - newPos;
    dir.normalise();
    camRot.pitch += dir.y * (90.f*deltaTime);
    camRot.yaw += dir.x * (-90.f*deltaTime);

    //DebugOutput(L"oldPos(%f, %f)\n", oldPos.x, oldPos.y);
    //DebugOutput(L"newPos(%f, %f)\n", newPos.x, newPos.y);
    //DebugOutput(L"dir(%f, %f)\n", dir.x, dir.y);
    //DebugOutput(L"CamRot - Roll: %f - Pitch: %f - Yaw: %f\n", camRot.roll, camRot.pitch, camRot.yaw);
    //DebugOutput(L"#EndFrame\n");

    camera->SetPitch(camRot.pitch);
    camera->SetYaw(camRot.yaw);
    camera->Update();
}

// Render Objects
void TestScene::Render()
{
    // For extended functionality
    LegacyOpenGLRenderer* loglRenderer = static_cast<LegacyOpenGLRenderer*>(renderer);

    //glClearColor(deltaTime, deltaTime, deltaTime, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (!wireframeMode)
    {
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    else
    {
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    renderer->Look();

    // Directional Light
    static GLfloat dirLight_Diffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    static GLfloat dirLight_Position[] = { 1.0f, 1.0f, 0.25f, 0.0f };
    loglRenderer->SetLightProperty(GL_LIGHT0, GL_DIFFUSE, dirLight_Diffuse);
    loglRenderer->SetLightProperty(GL_LIGHT0, GL_POSITION, dirLight_Position);
    loglRenderer->EnableLight(GL_LIGHT0);
    
    for (auto& obj : objects)
    {
        //loglRenderer->DrawObject(*obj);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, model.getTextureID());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
        glScalef(0.25f, 0.25f, 0.25f);
        glVertexPointer(3, GL_FLOAT, 0, &(model.getVerts()[0]) );
        glNormalPointer(GL_FLOAT, 0, &(model.getNorms()[0]) );
        glTexCoordPointer(2, GL_FLOAT, 0, &(model.getTexCoords()[0]) );
        glDrawArrays(GL_TRIANGLES, 0, model.getVerts().size()/3);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, NULL);
    glDisable(GL_TEXTURE_2D);
}

// UVs whacky
void TestScene::SetupCube36(Object* cube)
{
    auto loglRenderer = static_cast<LegacyOpenGLRenderer*>(renderer);
    std::vector<Vector3> cubeVerts;
    cubeVerts.reserve(36);
    cubeVerts =
    {{
        // Front
        Vector3(-0.5f, 0.5f, -0.5f),    // 1
        Vector3(0.5f, -0.5f, -0.5f),    // 2
        Vector3(-0.5f, -0.5f, -0.5f),   // 3
        Vector3(-0.5f, 0.5f, -0.5f),    // 4
        Vector3(0.5f, 0.5f, -0.5f),     // 5
        Vector3(0.5f, -0.5f, -0.5f),    // 6
                                        // Bottom
        Vector3(-0.5f, -0.5f, -0.5f),   // 7
        Vector3(0.5f, -0.5f, 0.5f),     // 8
        Vector3(-0.5f, -0.5f, 0.5f),    // 9
        Vector3(0.5f, -0.5f, -0.5f),    // 10
        Vector3(0.5f, -0.5f, 0.5f),     // 11
        Vector3(-0.5f, -0.5f, -0.5f),   // 12
                                        // Back
        Vector3(-0.5f, -0.5f, 0.5f),    // 13
        Vector3(0.5f, 0.5f, 0.5f),      // 14
        Vector3(-0.5f, 0.5f, 0.5f),     // 15
        Vector3(-0.5f, -0.5f, 0.5f),    // 16
        Vector3(0.5f, -0.5f, 0.5f),     // 17
        Vector3(0.5f, 0.5f, 0.5f),      // 18
                                        // Top
        Vector3(-0.5f, 0.5f, 0.5f),     // 19
        Vector3(0.5f, 0.5f, 0.5f),      // 20
        Vector3(-0.5f, 0.5f, -0.5f),    // 21
        Vector3(0.5f, 0.5f, 0.5f),      // 22
        Vector3(0.5f, 0.5f, -0.5f),     // 23
        Vector3(-0.5f, 0.5f, -0.5f),    // 24
                                        // Left
        Vector3(-0.5f, 0.5f, -0.5f),    // 25
        Vector3(-0.5f, -0.5f, -0.5f),   // 26
        Vector3(-0.5f, 0.5f, 0.5f),     // 27
        Vector3(-0.5f, -0.5f, 0.5f),    // 28
        Vector3(-0.5f, 0.5f, 0.5f),     // 29
        Vector3(-0.5f, -0.5f, -0.5f),   // 30
                                        // Right
        Vector3(0.5f, -0.5f, -0.5f),    // 31
        Vector3(0.5f, 0.5f, -0.5f),     // 32
        Vector3(0.5f, -0.5f, 0.5f),     // 33
        Vector3(0.5f, 0.5f, -0.5f),     // 34
        Vector3(0.5f, 0.5f, 0.5f),      // 35
        Vector3(0.5f, -0.5f, 0.5f)      // 36
    }};

    std::vector<Vector3> cubeColours;
    cubeColours.reserve(36);
    for (int i = 0; i < 36; i++) cubeColours.push_back(Vector3(1.0f, 1.0f, 1.0f));
    /*cubeColours =
    {{
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),

        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),
        Vector3(1.0f, 0.0f, 0.0f),

        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),

        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),

        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),

        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),

        Vector3(1.0f, 0.0f, 1.0f),
        Vector3(1.0f, 0.0f, 1.0f),
        Vector3(1.0f, 0.0f, 1.0f),

        Vector3(1.0f, 0.0f, 1.0f),
        Vector3(1.0f, 0.0f, 1.0f),
        Vector3(1.0f, 0.0f, 1.0f),

        Vector3(0.0f, 1.0f, 1.0f),
        Vector3(0.0f, 1.0f, 1.0f),
        Vector3(0.0f, 1.0f, 1.0f),

        Vector3(0.0f, 1.0f, 1.0f),
        Vector3(0.0f, 1.0f, 1.0f),
        Vector3(0.0f, 1.0f, 1.0f),

        Vector3(1.0f, 1.0f, 0.0f),
        Vector3(1.0f, 1.0f, 0.0f),
        Vector3(1.0f, 1.0f, 0.0f),

        Vector3(1.0f, 1.0f, 0.0f),
        Vector3(1.0f, 1.0f, 0.0f),
        Vector3(1.0f, 1.0f, 0.0f)
    }};*/

    std::vector<Vector3> cubeNormals;
    cubeNormals.reserve(36);
    cubeNormals = 
    {{
            // Front
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
        Vector3(0.0f, 0.0f, -1.0f),
            // Bottom
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
        Vector3(0.0f, -1.0f, 0.0f),
            // Back
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
        Vector3(0.0f, 0.0f, 1.0f),
            // Top
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
        Vector3(0.0f, 1.0f, 0.0f),
            // Left
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
        Vector3(-1.0f, 0.0f, 0.0f),
            // Right
        Vector3(1.0f, 0.0f, 0.0),
        Vector3(1.0f, 0.0f, 0.0),
        Vector3(1.0f, 0.0f, 0.0),
        Vector3(1.0f, 0.0f, 0.0),
        Vector3(1.0f, 0.0f, 0.0),
        Vector3(1.0f, 0.0f, 0.0)
    }};

    std::vector<Vector2> cubeUVs;
    cubeUVs.reserve(36);
    cubeUVs =
    {{
            // Front
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 0.0f),

        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 0.0f),
        // Bottom
        Vector2(0.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 0.0f),

        Vector2(1.0f, 1.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 1.0f),
            // Back
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 1.0f),

        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 1.0f),
            // Top
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 1.0f),

        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 1.0f),
            // Left
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 0.0f),

        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 1.0f),
            // Right
        Vector2(0.0f, 1.0f),
        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 1.0f),

        Vector2(0.0f, 0.0f),
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f)
    }};

    cube->setGLMode(GL_TRIANGLES);
    cube->setColours(cubeColours);
    cube->setNormals(cubeNormals);
    cube->setUVs(cubeUVs);
    cube->setVerts(cubeVerts);

    GLuint tex = loglRenderer->LoadTexture("crate.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

    Texture texture(tex);
    texture.setUProperty(GL_REPEAT);
    texture.setVProperty(GL_REPEAT);

    cube->setTexture(texture);
    cube->enableTexture();

    cube->setPosition(Vector3(0.0f, 0.0f, 0.0f));
    cube->setScale(Vector3(1.0f, 1.0f, 1.0f));
    cube->setRotation(Rotation(0.0f, Vector3(0.0f, 1.0f, 0.0f))); 
}
