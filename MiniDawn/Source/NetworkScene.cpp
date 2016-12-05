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
    lockMouse = false;
    ShowCursor(!lockMouse);

    // Create free camera
    newPos = Vector2(0.0f, 0.0f);
    camRot = { 0.f, 0.f, 0.f };
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
    networkSystem = MakeUnique<NetworkSystem>();
    networkSystem->Start();
}

void NetworkScene::SetupObjects()
{
    players.resize(16);

    spObject player(new Object);
    player->SetVO(MakeShareable(new PrimitiveUVSphere(128, 64)));
    player->EnableTexture();
    Texture playerTexture;
    LoadTexture(playerTexture, "textures/smiley.png");
    player->GetVO()->GetTexture() = playerTexture; // Awkward
    player->SetGLMode(GL_TRIANGLES);
    player->SetName(L"Player");
    objects.push_back(player);
    objectMap.insert(std::make_pair(player->GetName(), player));
    players[0] = player;

    for (int i = 1; i < 16; i++)
    {
        spObject otherPlayer(new Object);
        otherPlayer->SetVO(MakeShareable(new PrimitiveUVSphere(128, 64)));
        otherPlayer->EnableTexture();
        otherPlayer->GetVO()->GetTexture() = playerTexture;
        otherPlayer->SetGLMode(GL_TRIANGLES);
        otherPlayer->SetName(reinterpret_cast<wchar_t*>(&i)); // This'll just create garbage, but we shouldn't have to look them up manually anyway
        objects.push_back(otherPlayer);
        objectMap.insert(std::make_pair(otherPlayer->GetName(), otherPlayer));
        players[i] = otherPlayer;
    }
}

void NetworkScene::SortPlayersById()
{
    spObject player = players[0]; 
    players.erase(players.begin());
    players.insert(players.begin()+playerId, player);
    playersSorted = true;
}

void NetworkScene::Update()
{
    if (inputSystem->IsKeyDown(VK_ESCAPE))
    {
        networkSystem->Shutdown();
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
   // std::cout << "PlayerPos: (" << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << ")" << std::endl;

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
#ifdef _DEBUG
    //std::cout << "CamRot - Yaw: " << camRot.yaw << " Pitch: " << camRot.pitch << " Roll: " << camRot.roll << std::endl;
#endif

    // Update
    camera->SetPosition(camPos);
    camera->SetCameraRotation(camRot);
    camera->Update();

    if (networkSystem->ConnectedAndIdentified()) 
    {
        networkSystem->SetMyRecord(objectMap[L"Player"]->GetTransform()); 
        // The data that we put in here can actually be overwritten immediately when the network ticks, 
        // because of a snapshot or the server correcting us
    }
    networkSystem->Tick();

    if (networkSystem->ConnectedAndIdentified())
    {
        playerId = networkSystem->GetMyId();
        if (!playersSorted) { SortPlayersById(); }
        auto playerRecords = networkSystem->GetPlayerRecords();
        auto oldPlayerRecords = networkSystem->GetOldPlayerRecords();
        auto playerRecordHistory = networkSystem->GetPlayerRecordHistory(); // So we know how much to weight the old player record against the latest
        auto activePlayers = networkSystem->GetActivePlayers();

        int i = 0;
        for (auto &record : playerRecords)
        {
            if (activePlayers[i])
            {
                if (playerRecordHistory[i].UpdatedThisFrame) // No point recalculating if the data hasn't changed
                {
                    // Do some direction finding
                    PlayerRecord current, old;
                    current = playerRecords[i];
                    old = oldPlayerRecords[i];
                    // Set some temporary variables to hold our interpolated values
                    Vector3 currentPos, oldPos, delta;
                    delta = oldPos - currentPos;
                    playerDirections[i] = delta;
                }
                if (!networkSystem->SnapshotReceivedThisFrame() 
                && i != playerId) // We don't want to move ourselves twice
                {
                    // Move the players 
                    Transform transform = players[i]->GetTransform();
                    Vector3 pos = transform.GetPosition();
                    pos += playerDirections[i] * deltaTime * playerSpeed; // Player speed is constant so we don't need to worry about acceleration
                    transform.SetPosition(pos);
                    players[i]->SetTransform(transform);
#ifdef _DEBUG
                    std::cout << static_cast<char>(i + 48) << ": transform pos (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
#endif
                }
                else if(networkSystem->SnapshotReceivedThisFrame()) // Put everyone where the server says they are, might feel jerky if there's a big difference
                {
                    players[i]->SetTransform(record.transform);
#ifdef _DEBUG
                    Vector3 pos = record.transform.GetPosition();
                    std::cout << static_cast<char>(i + 48) << ": transform pos (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
#endif
                }
            }
            i++;
        }
    }
}

void NetworkScene::Render()
{
    LegacyOpenGLRenderer *lOGLRenderer = static_cast<LegacyOpenGLRenderer*>(renderer); // Completely break the point of having a generic renderer

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

    // Point light at the centre of the scene (totally not a sun, don't look at me like that)
    static GLfloat pointLight_Ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    static GLfloat pointLight_Diffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
    static GLfloat pointLight_Position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_AMBIENT, pointLight_Ambient);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_DIFFUSE, pointLight_Diffuse);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_POSITION, pointLight_Position);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.005f);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);
    lOGLRenderer->EnableLight(GL_LIGHT2);

    drawQueue = GetDrawQueue(objects);
    while (!drawQueue.empty())
    {
        renderer->DrawObject(*drawQueue.top());
        drawQueue.pop();
    }
}
