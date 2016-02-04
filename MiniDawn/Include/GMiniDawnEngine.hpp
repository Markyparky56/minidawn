#pragma once
class MiniDawnEngine
{
public:
    MiniDawnEngine();
    ~MiniDawnEngine();

    int PreInit();
    int Init();
    int Tick();
    int Exit();

    bool IsRunning();
private:
    bool isRunning;
} GMiniDawnEngine;