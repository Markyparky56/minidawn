#pragma once
#include <memory> 
#include "GenericMessageHandler.hpp"

using std::shared_ptr;

class GenericApplication
{
public:
    GenericApplication() {}
    virtual ~GenericApplication() {}

    // Message handler
    virtual void setMessageHandler(const shared_ptr<GenericMessageHandler>& _messageHandler) { MessageHander = _messageHandler; }
    virtual shared_ptr<GenericMessageHandler> getMessageHandler() { return MessageHandler; }
    // Window creation

    // Tick

    // Destroy

protected:
    // Message Handler
    
    shared_ptr<GenericMessageHandler> MessageHandler;
};