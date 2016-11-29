#include "NetworkSystem.hpp"

void NetworkSystem::udpHandleReceive(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        // There should be a new message in the udpRecvBuffer
        UDPMessage *recvdMsg = reinterpret_cast<UDPMessage*>(udpRecvBuffer.c_array());
        // Send it down the message channel to be handled by the main loop
        udpMessageChannel.Write(*recvdMsg);
    }
}

void NetworkSystem::udpHandleSend(boost::shared_ptr<UDPMessage> msg, const boost::system::error_code & error, std::size_t bytesTransferred)
{
    udpBusy = false;
}

void NetworkSystem::tcpHandleReceive(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        // There should be a new message in the tcpRecvBuffer
        TCPMessage *recvdMsg = reinterpret_cast<TCPMessage*>(tcpRecvBuffer.c_array());
        // Send it down the message channel to be handled by the main loop
        tcpMessageChannel.Write(*recvdMsg);
    }
}

void NetworkSystem::tcpHandleSend(boost::shared_ptr<TCPMessage> msg, const boost::system::error_code & error, std::size_t bytesTransferred)
{
    tcpBusy = false;
}
