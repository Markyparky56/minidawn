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

void TestScene::Setup()
{
    windowDef = parentApp->GetActiveWindow()->GetDef();
    screenCentre = Vector2(windowDef->Width / 2, windowDef->Height / 2);

    buttonPressGracePeriod = 0.2f;
    lockMouseLastChangeTime = static_cast<float>(gameTime);
    wireframeModeLastChangeTime = static_cast<float>(gameTime);
    rotationActiveLastChangeTime = static_cast<float>(gameTime);
    lockMouse = true;
    fixedCamEnabled = false;
    ShowCursor(!lockMouse);
    
    // Create our free camera
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

    fixedCam = MakeUnique<Camera>();

    // Create the skysphere
    skypshere = MakeUnique<Skysphere>(128, 64);
    Texture skyTexture;
    LoadTexture(skyTexture, "textures/starsphere.png");
    skypshere->SetSkysphereTexture(skyTexture);

    SetupObjects();    
}

// Create Objects
void TestScene::SetupObjects()
{    
    spObject sun(new Object);
    sun->SetVO(MakeShareable(new PrimitiveUVSphere(256, 128)));
    sun->GetTransform().SetScale(Vector3(2.f, 2.f, 2.f));
    Texture sunTexture;
    LoadTexture(sunTexture, "textures/texture_sun.png");
    sun->GetVO()->GetTexture() = sunTexture;
    sun->SetGLMode(GL_TRIANGLES);
    sun->EnableTexture();
    sun->SetName(L"Sun");
    objects.push_back(sun);
    objectMap.insert(std::make_pair(sun->GetName(), sun));

    spObject world(new Object);
    world->SetVO(MakeShareable(new PrimitiveUVSphere(256, 128)));
    world->GetTransform().SetPosition(Vector3(5.f, 0.f, 0.f));
    Texture worldTexture;
    LoadTexture(worldTexture, "textures/4kworld.png");
    world->GetVO()->GetTexture() = worldTexture;
    world->SetGLMode(GL_TRIANGLES);
    world->EnableTexture();
    world->SetName(L"Earth");
    world->SetParent(sun.Get(), TransformInheritance(true, false, true, false));
    objects.push_back(world);
    objectMap.insert(std::make_pair(world->GetName(), world));


    spObject clouds(new Object);
    clouds->SetVO(MakeShareable(new PrimitiveUVSphere(256, 128)));
    clouds->GetTransform().SetScale(Vector3(1.01f, 1.01f, 1.01f));
    Texture cloudTexture;
    LoadTexture(cloudTexture, "textures/clouds.png");
    clouds->GetVO()->GetTexture() = cloudTexture;
    clouds->SetGLMode(GL_TRIANGLES);
    clouds->EnableTexture();
    clouds->EnableTransparency();
    clouds->SetName(L"Clouds");
    clouds->SetParent(world.Get(), TransformInheritance(true, false, true, false));
    objects.push_back(clouds);
    objectMap.insert(std::make_pair(clouds->GetName(), clouds));

    //spObject disc(new Object);
    //disc->SetVO(new PrimitiveDisc(64));
    //Texture discTexture;
    //LoadTexture(discTexture, "crate.png");
    //disc->GetVO()->GetTexture() = discTexture;
    //disc->SetGLMode(GL_TRIANGLE_FAN);
    //disc->GetTransform().SetPosition(Vector3(2.5f, 0.0f, 0.0f));
    //disc->EnableTexture();
    //disc->SetName(L"Disc");
    //objects.push_back(disc);

    spObject moon(new Object);
    moon->SetVO(MakeShareable(new PrimitiveUVSphere(256, 128)));
    moon->GetTransform().SetScale(Vector3(0.27f, 0.27f, 0.27f));
    moon->GetTransform().SetPosition(Vector3(0.f, 0.f, -2.2f));
    Texture moonTexture;
    LoadTexture(moonTexture, "textures/moonmap1k.png");
    moon->GetVO()->GetTexture() = moonTexture;
    moon->SetGLMode(GL_TRIANGLES);
    moon->EnableTexture();
    moon->SetName(L"Moon");
    moon->SetParent(world.Get(), TransformInheritance(true, false, true, false));
    objects.push_back(moon);
    objectMap.insert(std::make_pair(moon->GetName(), moon));

    spObject hst(new Object);
    hst->SetVO(MakeShareable(new Model));
    static_cast<Model*>(hst->GetVO().Get())->Load("models/hst.obj", "textures/hst.png");
    hst->GetTransform().SetScale(Vector3(0.0001f, 0.0001f, 0.0001f)); // This thing is yuge!
    hst->GetTransform().SetPosition(Vector3(-1.1f, 0.f, 0.f));
    hst->GetTransform().SetPreRotation(Rotation(0.f, Vector3(0.25f, 0.25f, -0.25f)));
    hst->GetTransform().SetRotation(Rotation(0.f, Vector3(-0.2f, 0.8f, -0.2f)));
    hst->SetGLMode(GL_TRIANGLES);
    hst->SetName(L"HST");
    hst->EnableTexture();
    hst->SetParent(world.Get(), TransformInheritance(true, false, true, false));
    objects.push_back(hst);
    objectMap.insert(std::make_pair(hst->GetName(), hst));

    spObject iss(new Object);
    iss->SetVO(MakeShareable(new Model));
    static_cast<Model*>(iss->GetVO().Get())->Load("models/iss.obj", "textures/iss.png");
    iss->GetTransform().SetScale(Vector3(0.01f, 0.01f, 0.01f));
    iss->GetTransform().SetPreRotation(Rotation(0.f, Vector3(0.f, 0.5f, 0.f)));
    iss->GetTransform().SetRotation(Rotation(0.f, Vector3(0.25f, 0.5f, 0.25f)));
    iss->GetTransform().SetPosition(Vector3(0.f, 0.f, 1.2f));
    iss->SetGLMode(GL_TRIANGLES);
    iss->SetName(L"ISS");
    iss->EnableTexture();
    iss->SetParent(world.Get(), TransformInheritance(true, false, true, false));
    objects.push_back(iss);
    objectMap.insert(std::make_pair(iss->GetName(), iss));

    spObject teapot(new Object);
    teapot->SetVO(MakeShareable(new Model));
    static_cast<Model*>(teapot->GetVO().Get())->Load("Models/teapot.obj", "textures/teapot.png");
    teapot->GetTransform().SetPosition(Vector3(5.f, 5.f, -3.f));
    teapot->GetTransform().SetPreRotation(Rotation(0.f, Vector3(1.f, 0.f, 0.f)));
    teapot->GetTransform().SetRotation(Rotation(0.f, Vector3(0.f, 0.f, 1.f)));
    teapot->GetTransform().SetScale(Vector3(0.05f, 0.05f, 0.05f));
    teapot->SetGLMode(GL_TRIANGLES);
    teapot->SetName(L"Teapot");
    teapot->EnableTexture();
    teapot->SetParent(iss.Get(), TransformInheritance(true, true, true, false));
    objects.push_back(teapot);
    objectMap.insert(std::make_pair(teapot->GetName(), teapot));
}

// Logic
void TestScene::Update()
{
    //Rotation oldRot = objects[0]->GetTransform().GetRotation();
    //oldRot.deg += -15.0f * deltaTime;
    //objects[0]->GetTransform().SetRotation(oldRot);

    // Handle input

    if (inputSystem->IsKeyDown(VK_ESCAPE)) // Hit escape to exit, so if we've locked the mouse we can still quit
    {
        GMiniDawnEngine.RequestEnd();
    }

    if (inputSystem->IsKeyDown('P'))
    {
        if (gameTime >= (wireframeModeLastChangeTime + buttonPressGracePeriod))
        {
            wireframeMode = !wireframeMode;
            wireframeModeLastChangeTime = static_cast<float>(gameTime);
        }
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
    if (inputSystem->IsKeyDown('O'))
    {
        if (gameTime >= (rotationActiveLastChangeTime + buttonPressGracePeriod))
        {
            rotationActive = !rotationActive;
            rotationActiveLastChangeTime = static_cast<float>(gameTime);
        }
    }
    if (inputSystem->IsKeyDown('I'))
    {
        if (gameTime >= (fixedCamEnabledLastChangeTime + buttonPressGracePeriod))
        {
            fixedCamEnabled = !fixedCamEnabled;
            fixedCamEnabledLastChangeTime = static_cast<float>(gameTime);
            if (fixedCamEnabled)
            {
                renderer->SetCamera(fixedCam.Get());
                DrawQueueSort::cam = fixedCam.Get();
            }
            else
            {
                renderer->SetCamera(camera.Get());
                DrawQueueSort::cam = fixedCam.Get();
            }
        }
    }

    if (fixedCamEnabled)
    {
        // Position the camera above the Sun
        auto sun = objectMap[L"Sun"];
        fixedCam->SetPosition(sun->GetTransform().GetPosition() + Vector3(0.f, 3.f, 0.f));
        // Grab the mouse position
        newPos.x = static_cast<float>(inputSystem->GetMouseX());
        newPos.y = static_cast<float>(inputSystem->GetMouseY());

        // Convert mouse movement to camera rotation
        Vector2 dir = screenCentre - newPos;
        dir.normalise();
        camRot.pitch += dir.y * (90.f*deltaTime);
        camRot.yaw += dir.x * (-90.f*deltaTime);

        // Reset the mouse position
        if (lockMouse)
        {
            POINT pt;
            pt.x = static_cast<LONG>(screenCentre.x);
            pt.y = static_cast<LONG>(screenCentre.y);
            ClientToScreen(static_cast<HWND>(parentApp->GetActiveWindow()->getHwnd()), &pt);
            SetCursorPos(pt.x, pt.y);
        }

        fixedCam->SetPitch(camRot.pitch);
        fixedCam->SetYaw(camRot.yaw);
        fixedCam->SetRoll(camRot.roll);
        fixedCam->Update();
    }
    else
    {
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
        // Grab the mouse position
        newPos.x = static_cast<float>(inputSystem->GetMouseX());
        newPos.y = static_cast<float>(inputSystem->GetMouseY());

        // Convert mouse movement to camera rotation
        Vector2 dir = screenCentre - newPos;
        dir.normalise();
        camRot.pitch += dir.y * (90.f*deltaTime);
        camRot.yaw += dir.x * (-90.f*deltaTime);

        // Reset the mouse position
        if (lockMouse)
        {
            POINT pt;
            pt.x = static_cast<LONG>(screenCentre.x);
            pt.y = static_cast<LONG>(screenCentre.y);
            ClientToScreen(static_cast<HWND>(parentApp->GetActiveWindow()->getHwnd()), &pt);
            SetCursorPos(pt.x, pt.y);
        }

        camera->SetPosition(camPos);
        camera->SetPitch(camRot.pitch);
        camera->SetYaw(camRot.yaw);
        camera->SetRoll(camRot.roll);
        camera->Update();
    }

    // Degrees per second
    const float sunRotation       = 15.f,
                earthPreRotation  = 5.f,
                earthRotation     = 10.f, 
                earthTexShift     = 0.125f,
                cloudsTexShift    = 0.05f,
                moonPreRotation   = 15.f,
                moonRotation      = -15.f,
                hstPreRotation    = 5.f,
                hstRotation       = 15.f, 
                issPreRotation    = 5.f,
                issRotation       = 15.f,
                teapotPreRotation = 45.f,
                teapotRotation    = 15.f;

    if (rotationActive)
    {
        // Rotate the sun
        objectMap[L"Sun"]->GetTransform().GetRotation().deg += sunRotation*deltaTime;

        // Rotate the earth around the sun
        auto earth = objectMap[L"Earth"];
        Vector3 earthTexOffset = earth->GetVO()->GetTexture().GetOffset();
        earth->GetTransform().GetPreRotation().deg += earthPreRotation*deltaTime;
        earth->GetTransform().GetRotation().deg += earthRotation*deltaTime;
        earth->GetVO()->GetTexture().SetOffset(earthTexOffset + Vector3(earthTexShift*deltaTime, 0.f, 0.f));

        // Slowly shift the cloud texture
        auto clouds = objectMap[L"Clouds"];
        Vector3 cloudOffset = clouds->GetVO()->GetTexture().GetOffset();
        clouds->GetVO()->GetTexture().SetOffset(cloudOffset + Vector3(cloudsTexShift*deltaTime, 0.0f, 0.0f));

        //Rotate the moon around the earth
        auto moon = objectMap[L"Moon"];
        moon->GetTransform().GetRotation().deg += moonRotation*deltaTime;
        moon->GetTransform().GetPreRotation().deg += moonPreRotation*deltaTime;

        // Rotate the HST around the earth
        auto hst = objectMap[L"HST"];
        hst->GetTransform().GetRotation().deg += hstRotation*deltaTime;
        hst->GetTransform().GetPreRotation().deg += hstPreRotation*deltaTime;

        // Rotate the ISS around the earth
        auto iss = objectMap[L"ISS"];
        iss->GetTransform().GetRotation().deg += issRotation*deltaTime;
        iss->GetTransform().GetPreRotation().deg += issPreRotation*deltaTime;

        // Rotate the teapot around the ISS
        auto teapot = objectMap[L"Teapot"];
        teapot->GetTransform().GetRotation().deg += teapotRotation*deltaTime;
        teapot->GetTransform().GetPreRotation().deg += teapotPreRotation*deltaTime;
    }

    //Rotation discRotation = objects[2]->GetTransform().GetRotation();
    //discRotation.deg += 45.0f * deltaTime;
    //objects[2]->GetTransform().SetRotation(discRotation); // Explicitly accessing an object is not advisable, since any change can throw it off
}

// Render Objects
void TestScene::Render()
{
    // For extended functionality
    LegacyOpenGLRenderer* lOGLRenderer = static_cast<LegacyOpenGLRenderer*>(renderer);

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
    glDisable(GL_LIGHT2);

    static GLfloat fullAmbientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    lOGLRenderer->SetLightProperty(GL_LIGHT1, GL_AMBIENT, fullAmbientLight);
    glEnable(GL_LIGHT1);
    renderer->DrawSkysphere(*skypshere);
    glDisable(GL_LIGHT1);

    // Directional Light
    static GLfloat dirLight_Diffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    static GLfloat dirLight_Position[] = { 1.0f, 1.0f, 0.25f, 0.0f };
    lOGLRenderer->SetLightProperty(GL_LIGHT0, GL_DIFFUSE, dirLight_Diffuse);
    lOGLRenderer->SetLightProperty(GL_LIGHT0, GL_POSITION, dirLight_Position);
    lOGLRenderer->EnableLight(GL_LIGHT0);

    // Point light from the sun
    static GLfloat pointLight_Ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    static GLfloat pointLight_Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static GLfloat pointLight_Position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_AMBIENT, pointLight_Ambient);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_DIFFUSE, pointLight_Diffuse);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_POSITION, pointLight_Position);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.125f);
    lOGLRenderer->SetLightProperty(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);
    lOGLRenderer->EnableLight(GL_LIGHT2);
    
    glColor3f(1.0f, 1.0f, 1.0f);

    // Assemble draw queue
    drawQueue = GetDrawQueue(objects);
    while (!drawQueue.empty())
    {
        //DebugOutput(L"Draw Queue Length: %d\n", drawQueue.size());
        //DebugOutput(L"Drawing object: %s\n", drawQueue.top()->GetName().c_str());
        renderer->DrawObject(*drawQueue.top());        
        drawQueue.pop();        
    }
}
