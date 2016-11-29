#include "NetworkSystem.hpp"

void NetworkSystem::udpHandleResolve(const boost::system::error_code & error, udp::resolver::iterator endpointIter)
{
    if (!error)
    {
        // Take the first endpoint and set udpEndpoint
        udpEndpoint = *endpointIter;

        udpSocket = udp::socket(io_service);
        udpSocket.open(udp::v4()); // We need our own socket so we can listen to what the server wants to send us
    }
    else
    {
        // Resolve failed for some reason
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        abort();
#endif
    }
}

void NetworkSystem::udpHandleReceive(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        // There should be a new message in the udpRecvBuffer
        UDPMessage *recvdMsg = reinterpret_cast<UDPMessage*>(udpRecvBuffer.c_array());
        // Send it down the message channel to be handled by the main loop
        udpMessageChannel.Write(*recvdMsg);
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        abort();
#endif
    }
}

void NetworkSystem::udpHandleSend(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    udpBusy = false;
}

void NetworkSystem::tcpHandleResolve(const boost::system::error_code & error, tcp::resolver::iterator endpointIter)
{
    if (!error)
    {
        // Attempt a connection to the first endpoint in the list
        tcpEndpointIterator = endpointIter;
        tcp::endpoint endpoint = *tcpEndpointIterator; // dereference the iterator for the first endpoint
        tcpSocket.async_connect(
            endpoint,
            boost::bind(&NetworkSystem::tcpHandleConnect, this, boost::asio::placeholders::error)
        );
    }
    else
    {
        // Resolve failed for some reason
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        abort();
#endif
    }
}

void NetworkSystem::tcpHandleConnect(const boost::system::error_code & error)
{
    if (!error)
    {
        // The connection was successful!
        tcpBusy = false; // We can now send stuff!
    }
    else if (tcpEndpointIterator != tcp::resolver::iterator())
    {
        // The connection failed, try the next endpoint in the list
        tcpSocket.close();
        tcp::endpoint endpoint = *++tcpEndpointIterator;// Advance and dereference
        tcpSocket.async_connect(
            endpoint,
            boost::bind(&NetworkSystem::tcpHandleConnect, this, boost::asio::placeholders::error)
        );
    }
    else
    {
        // Probably failed on all endpoints
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        abort();
#endif
    }
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
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        abort();
#endif
    }
}

void NetworkSystem::tcpHandleSend(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    tcpBusy = false;
}
