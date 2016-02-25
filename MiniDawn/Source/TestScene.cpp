#include "TestScene.hpp"

// Create Objects
void TestScene::Setup()
{
    Object* cube = new Object;
    SetupCube36(cube);
    objects.push_back(pObject(cube));

    camera = std::make_unique<Camera>();
    camera->SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    camera->SetUp(Vector3(0.0f, 1.0f, 0.0f));
    camera->Update();
    renderer->SetCamera(camera.get());
}

//void TestScene::Init(InputSystem * InInputSystem, Renderer * InRenderer)
//{
//    inputSystem = InInputSystem;
//    renderer = InRenderer;
//    Setup();
//}
//
//void TestScene::Tick(float InDeltaTime)
//{
//    deltaTime = InDeltaTime;
//    gameTime += deltaTime;
//    Update();
//    Render();
//}

// Logic
void TestScene::Update()
{
}

// Render Objects
void TestScene::Render()
{
    LegacyOpenGLRenderer* loglRenderer = static_cast<LegacyOpenGLRenderer*>(renderer);

    //glClearColor(deltaTime, deltaTime, deltaTime, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //renderer->Look();
    gluLookAt(0, 0, 6, 0, 0, 0, 0, 1, 0); //Where we are, What we look at, and which way is up

    //// Directional Light
    //static GLfloat dirLight_Diffuse[] = { 0.75f, 0.75f, 0.75f, 1.0f };
    //static GLfloat dirLight_Position[] = { 1.0f, 1.0f, 0.25f, 0.0f };
    //loglRenderer->SetLightProperty(GL_LIGHT0, GL_DIFFUSE, dirLight_Diffuse);
    //loglRenderer->SetLightProperty(GL_LIGHT0, GL_POSITION, dirLight_Position);
    //loglRenderer->EnableLight(GL_LIGHT0);
    
    glBegin(GL_TRIANGLES);

    glVertex3f(0.0, 1.0, 0.0);

    glVertex3f(-1.0, 0.0, 0.0);

    glVertex3f(1.0, 0.0, 0.0);

    glEnd();

    //for (auto& obj : objects)
    //{
    //    loglRenderer->DrawObject(*obj);
    //}
}

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
   /* cubeColours =
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
        Vector2(1.0f, 0.0f),
        Vector2(0.0f, 1.0f),
            // Top
        Vector2(1.0f, 0.0f),
        Vector2(1.0f, 1.0f),
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
