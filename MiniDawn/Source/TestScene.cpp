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

    camera = std::make_unique<Camera>();
    camera->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    camera->SetUp(Vector3(0.0f, 1.0f, 0.0f));
    camera->Update();
    renderer->SetCamera(camera.get());
    
    model.Load("models/teapot.obj", "crate.png");
}

// Logic
void TestScene::Update()
{
    /*Rotation oldRot = objects[0]->getRotation();
    oldRot.deg += 15 * deltaTime;
    objects[0]->setRotation(oldRot);*/

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
    
    if (inputSystem->IsKeyDown(VK_ESCAPE)) // Hit escape to exit, so if we decide to lock the mouse we can still quit
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

    //DebugOutput(L"oldPos(%f, %f)\n", oldPos.x, oldPos.y);
    //DebugOutput(L"newPos(%f, %f)\n", newPos.x, newPos.y);
    //DebugOutput(L"dir(%f, %f)\n", dir.x, dir.y);
    //DebugOutput(L"CamRot - Roll: %f - Pitch: %f - Yaw: %f\n", camRot.roll, camRot.pitch, camRot.yaw);
    //DebugOutput(L"#EndFrame\n");

    camera->SetPosition(camPos);
    camera->SetPitch(camRot.pitch);
    camera->SetYaw(camRot.yaw);
    camera->SetRoll(camRot.roll);
    camera->Update();

    Vector3 currOffset = model.GetTexture().GetOffset();
    model.GetTexture().SetOffset(currOffset + Vector3(deltaTime, 0.0f, 0.0f));
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
    
    glColor3f(1.0f, 1.0f, 1.0f);
    //glPushMatrix();
    //    glScalef(0.25f, 0.25f, 0.25f);
    //    glVertexPointer(3, GL_FLOAT, 0, &(model.getVerts()[0]) );
    //    glNormalPointer(GL_FLOAT, 0, &(model.getNorms()[0]) );
    //    glTexCoordPointer(2, GL_FLOAT, 0, &(model.getTexCoords()[0]) );
    //    glDrawArrays(GL_TRIANGLES, 0, model.getVerts().size()/3);
    //glPopMatrix();

    renderer->DrawModel(model);
}
