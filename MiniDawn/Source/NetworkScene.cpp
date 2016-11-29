#include "NetworkScene.hpp"
#include "GMiniDawnEngine.hpp"
#include "dbout.hpp"

Camera *DrawQueueSort::cam = nullptr;

// Possibly implement a texture manager
void LoadTexture(Texture& tex, char* filename)
{
    tex.SetTexture(SOIL_load_OGL_texture
    (
        filename,
        SOIL_LOAD_RGBA,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT /*| SOIL_FLAG_INVERT_Y*/
    ));

    if (tex.GetTexture() == 0)
    {
        DebugOutput(L"SOIL loading error: '%s'\n", SOIL_last_result());
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

void NetworkScene::Setup()
{
    windowDef = parentApp->GetActiveWindow()->GetDef();
    screenCentre = Vector2(windowDef->Width / 2, windowDef->Height / 2);

    buttonPressGracePeriod = 0.2f;
    lockMouseLastChangeTime = static_cast<float>(gameTime);
    lockMouse = true;
    ShowCursor(!lockMouse);

    // Create free camera
    newPos = Vector2(0.0f, 0.0f);
    camRot = { 0.f, 0.f, 0.f };
    camRot.pitch = 0.0f;
    camRot.yaw = 0.0f;
    camRot.roll = 0.0f;
    playerSpeed = 4.0f;
    playerRot = { 0.f, 0.f, 0.f };


    camera = MakeUnique<Camera>();
    camera->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    camera->SetUp(Vector3(0.0f, 1.0f, 0.0f));
    camera->SetCameraRotation( { 0.0f,0.0f,0.0f } ); // CamRot literal, roll, pitch and yaw all 0.0f
    camera->Update();
    renderer->SetCamera(camera.Get());
    DrawQueueSort::cam = camera.Get();

    firstPersonCam = MakeUnique<Camera>();

    // Create the skysphere
    skysphere = MakeUnique<Skysphere>(128, 64);
    Texture skyTexture;
    LoadTexture(skyTexture, "textures/starsphere.png");
    skysphere->SetSkysphereTexture(skyTexture);

    SetupObjects();
}

void NetworkScene::SetupObjects()
{
    spObject player(new Object);
    player->SetVO(MakeShareable(new PrimitiveUVSphere(128, 64)));
    player->DisableTexture();
    player->SetGLMode(GL_TRIANGLES);
    player->SetName(L"Player");
    objects.push_back(player);
    objectMap.insert(std::make_pair(player->GetName(), player));
}

void NetworkScene::Update()
{
    if (inputSystem->IsKeyDown(VK_ESCAPE))
    {
        GMiniDawnEngine.RequestEnd();
    }
    if (inputSystem->IsKeyDown(VK_F1))
    {
        if (gameTime >= (lockMouseLastChangeTime + buttonPressGracePeriod))
        {
            lockMouse = !lockMouse;
            ShowCursor(!lockMouse);
            lockMouseLastChangeTime = static_cast<float>(gameTime);
        }
    }

    // Player controls
    Vector3 playerPos = objectMap[L"Player"]->GetTransform().GetPosition();
    if (inputSystem->IsKeyDown('D'))
    {
        playerPos += (firstPersonCam->GetRight() * deltaTime * playerSpeed);
    }
    else if (inputSystem->IsKeyDown('A'))
    {
        playerPos -= (firstPersonCam->GetRight() * deltaTime * playerSpeed);
    }
    if (inputSystem->IsKeyDown('W'))
    {
        playerPos += (firstPersonCam->GetForward() * deltaTime * playerSpeed);
    }
    else if (inputSystem->IsKeyDown('S'))
    {
        playerPos -= (firstPersonCam->GetForward() * deltaTime * playerSpeed);
    }
    //if (inputSystem->IsKeyDown('E'))
    //{
    //    camRot.roll -= 90.f*deltaTime;
    //}
    //else if (inputSystem->IsKeyDown('Q'))
    //{
    //    camRot.roll += 90.f*deltaTime;
    //}
    if (inputSystem->IsKeyDown(VK_SPACE))
    {
        playerPos += (Vector3(0.0f, 1.0f, 0.0f) * deltaTime * playerSpeed);
    }
    else if (inputSystem->IsKeyDown(VK_CONTROL))
    {
        playerPos -= (Vector3(0.0f, 1.0f, 0.0f) * deltaTime * playerSpeed);
    }

    // Grab the mouse position
    newPos.x = static_cast<float>(inputSystem->GetMouseX());
    newPos.y = static_cast<float>(inputSystem->GetMouseY());

    // Convert mouse movement to rotation
    Vector2 dir = screenCentre - newPos;
    dir.normalise();
    playerRot.pitch += dir.y * (90.f*deltaTime);
    playerRot.yaw += dir.x * (-90.f*deltaTime);

    // Reset the mouse position
    if (lockMouse)
    {
        POINT pt;
        pt.x = static_cast<LONG>(screenCentre.x);
        pt.y = static_cast<LONG>(screenCentre.y);
        ClientToScreen(static_cast<HWND>(parentApp->GetActiveWindow()->getHwnd()), &pt);
        SetCursorPos(pt.x, pt.y);
    }

    // Set the first person camera to be inside the player
    firstPersonCam->SetPosition(playerPos);
    firstPersonCam->SetCameraRotation(playerRot);
    firstPersonCam->Update();

    // Work out the position of the camera behind the player
    Vector3 camPos = firstPersonCam->GetPosition();
    Vector3 playerForward = firstPersonCam->GetForward();
    Vector3 playerUp = firstPersonCam->GetUp();

    // Move back 2 units and up 2
    camPos = camPos - (2 * playerForward) + (2 * playerUp);
    
    // Look in the same direction as the first person camera
    camRot = firstPersonCam->GetYawPitchRoll();

    // Update
    camera->SetPosition(camPos);
    camera->SetCameraRotation(camRot);
    camera->Update();
}

void NetworkScene::Render()
{
    LegacyOpenGLRenderer *lOGLRenderer = static_cast<LegacyOpenGLRenderer*>(renderer); // Completely breaks the point of having a generic renderer

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPolygonMode(GL_FRONT, GL_FILL);

    renderer->Look();

    glDisable(GL_LIGHT1);
    static GLfloat fullAmbientLight[] = { 1.f, 1.f, 1.f, 1.f };
    lOGLRenderer->SetLightProperty(GL_LIGHT1, GL_AMBIENT, fullAmbientLight);
    glEnable(GL_LIGHT0);
    renderer->DrawSkysphere(*skysphere);
    glDisable(GL_LIGHT0);
    
    // Direction light
    static GLfloat dirLightDiffuse[] = { 0.75f, 0.75f, 0.75f, 1.f };
    static GLfloat dirLightPosition[] = { 1.f, 1.f, 0.25f, 0.0f };
    lOGLRenderer->SetLightProperty(GL_LIGHT1, GL_DIFFUSE, dirLightDiffuse);
    lOGLRenderer->SetLightProperty(GL_LIGHT1, GL_POSITION, dirLightPosition);
    lOGLRenderer->EnableLight(GL_LIGHT1);

    glColor3f(1.f, 1.f, 1.f); // I'm not sure what this is for...

    drawQueue = GetDrawQueue(objects);
    while (!drawQueue.empty())
    {
        renderer->DrawObject(*drawQueue.top());
        drawQueue.pop();
    }
}
