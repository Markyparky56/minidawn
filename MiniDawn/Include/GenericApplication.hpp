#pragma once
#include "SharedRef.hpp"
#include "GenericMessageHandler.hpp"
#include "GenericWindowDefinition.hpp"
#include "GenericWindow.hpp"

class InputSystem;
class Renderer;
class Scene;

class GenericApplication
{
public:
    GenericApplication() 
        : MessageHandler(MakeShareable(new GenericMessageHandler()))
    {
    }
    virtual ~GenericApplication() {}

    // Message handler
    virtual void setMessageHandler(const SharedRef<GenericMessageHandler>& _messageHandler) { MessageHandler = _messageHandler; }
    virtual SharedRef<GenericMessageHandler> getMessageHandler() { return MessageHandler; }
    // Window stuff
    virtual void PumpMessages(const float DeltaTime) {}
    virtual void Tick(const float DeltaTime) {}
    virtual SharedRef<GenericWindow> MakeWindow() { return MakeShareable(new GenericWindow()); } // Not sure how broken this will be
    virtual void InitialiseWindow(const SharedRef<GenericWindow>& Window, const SharedRef<GenericWindowDefinition>& InDefinition) {}
    virtual void DestroyApplication() {}
    //const SharedPtr<Cursor> Cursor;

    virtual void InitialiseScene(const SharedPtr<Scene>& InScene) {}
    virtual GenericWindow* GetActiveWindow() { return nullptr; }
protected:
    // Message Handler    
    SharedRef<class GenericMessageHandler> MessageHandler;
    SharedPtr<InputSystem> input;
    SharedPtr<Renderer> renderer;
};
