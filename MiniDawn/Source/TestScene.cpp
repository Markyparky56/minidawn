#include "TestScene.hpp"
#include "GMiniDawnEngine.hpp"
#ifdef _DEBUG
#include "dbout.hpp"
#endif

Camera* DrawQueueSort::cam = nullptr;

// Possibly implement a texture manager
void LoadTexture(Texture& tex, char* filename)
{
    tex.SetTexture(SOIL_load_OGL_texture
        (
            filename,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT /*| SOIL_FLAG_INVERT_Y*/
        ));

    if (tex.GetTexture() == 0)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
    }
}



std::priority_queue<spObject, std::deque<spObject>, DrawQueueSort> GetDrawQueue(std::vector<spObject>& objects)
{
    std::priority_queue<spObject, std::deque<spObject>, DrawQueueSort> drawQueue;
    for (auto obj : objects)
    {
        drawQueue.push(obj);
    }
    return drawQueue;
}

// Create Objects
void TestScene::Setup()
{
    windowDef = parentApp->GetActiveWindow()->GetDef();
    screenCentre = Vector2(windowDef->Width / 2, windowDef->Height / 2);

    buttonPressGracePeriod = 0.2f;
    lockMouseLastChangeTime = gameTime;
    wireframeModeLastChangeTime = gameTime;
    lockMouse = true;
    ShowCursor(!lockMouse);
    
    newPos = Vector2(0.0f, 0.0f);
    camRot.pitch = 0.0f;
    camRot.yaw = 0.0f;
    camRot.roll = 0.0f;
    cameraSpeed = 4.0f;

    camera = MakeUnique<Camera>();
    camera->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    camera->SetUp(Vector3(0.0f, 1.0f, 0.0f));
    camera->SetPitch(0.0f);
    camera->SetRoll(0.0f);
    camera->SetYaw(0.0f);
    camera->Update();
    renderer->SetCamera(camera.Get());
    DrawQueueSort::cam = camera.Get();

    
    skypshere = MakeUnique<Skysphere>(128, 64);
    Texture skyTexture;
    LoadTexture(skyTexture, "starsphere.png");
    skypshere->SetSkysphereTexture(skyTexture);

    //spObject obj(new Object);
    //objects.push_back(obj);
    //obj->SetVO(new Model);
    //bool loadResult = static_cast<Model*>(obj->GetVO())->Load("models/teapot.obj", "crate.png");
    //obj->EnableTexture();

    spObject world(new Object);
    world->SetVO(new PrimitiveUVSphere(256, 128));
    Texture sphereTexture;
    LoadTexture(sphereTexture, "4kworld.png");
    world->GetVO()->GetTexture() = sphereTexture;
    world->SetGLMode(GL_TRIANGLES);
    world->EnableTexture();
    world->SetName(L"World");
    objects.push_back(world);

    spObject disc(new Object);
    disc->SetVO(new PrimitiveDisc(64));
    Texture discTexture;
    LoadTexture(discTexture, "crate.png");
    disc->GetVO()->GetTexture() = discTexture;
    disc->SetGLMode(GL_TRIANGLE_FAN);
    disc->SetPosition(Vector3(2.5f, 0.0f, 0.0f));
    disc->EnableTexture();
    disc->SetName(L"Disc");
    objects.push_back(disc);
    //obj->SetGLMode(GL_TRIANGLES);
}

// Logic
void TestScene::Update()
{
    Rotation oldRot = objects[0]->GetRotation();
    oldRot.deg += -15.0f * deltaTime;
    objects[0]->SetRotation(oldRot);

    // Handle input
    if (inputSystem->IsKeyDown('P'))
    {
        if (gameTime >= (wireframeModeLastChangeTime + buttonPressGracePeriod))
        {
            wireframeMode = !wireframeMode;
            wireframeModeLastChangeTime = gameTime;
        }
    }
    if (inputSystem->IsKeyDown(VK_F1))
    {
        if (gameTime >= (lockMouseLastChangeTime + buttonPressGracePeriod))
        {
            lockMouse = !lockMouse;
            ShowCursor(!lockMouse);
            lockMouseLastChangeTime = gameTime;
        }
    }

    // Move Camera
    Vector3 camPos = camera->GetPosition();
    if (inputSystem->IsKeyDown('D'))
    {
        camPos += (camera->GetRight() * deltaTime * cameraSpeed);
    }
    else if (inputSystem->IsKeyDown('A'))
    {
        camPos -= (camera->GetRight() * deltaTime * cameraSpeed);
    }
    if (inputSystem->IsKeyDown('W'))
    {
        camPos += (camera->GetForward() * deltaTime * cameraSpeed);
    }
    else if (inputSystem->IsKeyDown('S'))
    {
        camPos -= (camera->GetForward() * deltaTime * cameraSpeed);
    }
    if (inputSystem->IsKeyDown('E'))
    {
        camRot.roll -= 90.f*deltaTime;
    }
    else if (inputSystem->IsKeyDown('Q'))
    {
        camRot.roll += 90.f*deltaTime;
    }
    if (inputSystem->IsKeyDown(VK_SPACE))
    {
        camPos += (Vector3(0.0f, 1.0f, 0.0f) * deltaTime * cameraSpeed);
    }
    else if (inputSystem->IsKeyDown(VK_CONTROL))
    {
        camPos -= (Vector3(0.0f, 1.0f, 0.0f) * deltaTime * cameraSpeed);
    }
    
    if (inputSystem->IsKeyDown(VK_ESCAPE)) // Hit escape to exit, so if we've locked the mouse we can still quit
    {
        GMiniDawnEngine.RequestEnd();
    }

    // Grab the mouse position
    newPos.x = inputSystem->GetMouseX();
    newPos.y = inputSystem->GetMouseY();

    // Convert mouse movement to camera rotation
    Vector2 dir = screenCentre - newPos;
    dir.normalise();
    camRot.pitch += dir.y * (90.f*deltaTime);
    camRot.yaw += dir.x * (-90.f*deltaTime);

    // Reset the mouse position
    if (lockMouse)
    {
        POINT pt;
        pt.x = screenCentre.x;
        pt.y = screenCentre.y;
        ClientToScreen(static_cast<HWND>(parentApp->GetActiveWindow()->getHwnd()), &pt);
        SetCursorPos(pt.x, pt.y);
    }

    camera->SetPosition(camPos);
    camera->SetPitch(camRot.pitch);
    camera->SetYaw(camRot.yaw);
    camera->SetRoll(camRot.roll);
    camera->Update();

    Vector3 currOffset = objects[0]->GetVO()->GetTexture().GetOffset();
   // objects[0]->GetVO()->GetTexture().SetOffset(currOffset + Vector3(deltaTime, 0.0f, 0.0f));

    Rotation discRotation = objects[1]->GetRotation();
    discRotation.deg += 45.0f * deltaTime;
    objects[1]->SetRotation(discRotation);
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

    glDisable(GL_LIGHT0);

    static GLfloat fullAmbientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    loglRenderer->SetLightProperty(GL_LIGHT1, GL_AMBIENT, fullAmbientLight);
    glEnable(GL_LIGHT1);
    renderer->DrawSkysphere(*skypshere);
    glDisable(GL_LIGHT1);

    // Directional Light
    static GLfloat dirLight_Diffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    static GLfloat dirLight_Position[] = { 1.0f, 1.0f, 0.25f, 0.0f };
    loglRenderer->SetLightProperty(GL_LIGHT0, GL_DIFFUSE, dirLight_Diffuse);
    loglRenderer->SetLightProperty(GL_LIGHT0, GL_POSITION, dirLight_Position);
    loglRenderer->EnableLight(GL_LIGHT0);
    
    glColor3f(1.0f, 1.0f, 1.0f);

    // Assemble draw queue
    drawQueue = GetDrawQueue(objects);
    while (!drawQueue.empty())
    {
        DebugOutput(L"Draw Queue Length: %d\n", drawQueue.size());
        DebugOutput(L"Drawing object: %s\n", drawQueue.top()->GetName().c_str());
        renderer->DrawObject(*drawQueue.top());        
        drawQueue.pop();        
    }
}
