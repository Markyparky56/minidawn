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

void NetworkScene::Update()
{

}

void NetworkScene::Render()
{

}

void NetworkScene::Setup()
{

}

void NetworkScene::SetupObjects()
{

}
