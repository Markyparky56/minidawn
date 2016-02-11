#pragma once
#include "SharedRef.hpp"
#include "GenericMessageHandler.hpp"

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
    // Window creation

    // Tick

    // Destroy

protected:
    // Message Handler
    
    SharedRef<class GenericMessageHandler> MessageHandler;
};
