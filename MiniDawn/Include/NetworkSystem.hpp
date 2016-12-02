#pragma once
/***************************************************************************************************
    A Networking System to manage a UDP and TCP connection to a remote host which will have
    authorative control over the true locations of objects in the scene.
    Will, on start up, connect via TCP to the server and syncronise with any other connected clients
    Will then regularly (e.g. 1/20th of a second) update the server of the local user's current position
    and direction (any any other important data pertaining to the user which is can fit in the pakcet)
    Will listen for UDP packets from the server (also sent regularly) for authorative updates which it 
    may need to correct local data (to the best of it's ability, minimising unrealistic movements
    which may cause rubberbanding or breaking of reality)
    Will listen on the original TCP connection for important messages from the server, like new clients
    connecting or disconnecting, snapshots of the (global) world (as opposed to local changes).
***************************************************************************************************/

#ifdef _DEBUG
#include <iostream>
#endif
#include "UniquePtr.hpp"
#include "SharedRef.hpp"
#include <stack> // UDP message stack, always deal with the newest message
#include <queue> // TCP message queue, messages received over TCP are important so deal with them in the order they arrive
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "Channel.hpp"
#include "Protocol.hpp"
#include <thread>

using pThread = UniquePtr<std::thread>;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class NetworkSystem : public boost::enable_shared_from_this<NetworkSystem>
{
public:
    NetworkSystem()         
        : udpSocket(io_service, udp::endpoint())
        , tcpSocket(io_service, tcp::endpoint())
    {     
        UDPInit();
        TCPInit();
    };
    ~NetworkSystem() {};

    bool UDPInit()
    {
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), "localhost", "4444");
        resolver.async_resolve(
            query, 
            boost::bind(&NetworkSystem::udpHandleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator)
        );               
    }

    void UDPSend(UDPMessage &msg)
    {
        memcpy(udpSendBuffer.c_array(), reinterpret_cast<uint8_t*>(&msg), sizeof(UDPMessage));
        udpSocket.async_send_to(
            boost::asio::buffer(udpSendBuffer),
            udpEndpoint,
            boost::bind(&NetworkSystem::udpHandleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
        udpBusy = true; // We'll flip this back off after it's sent
    }

    void UDPReceive()
    {
        udpSocket.async_receive_from(
            boost::asio::buffer(udpRecvBuffer),
            udpEndpoint,
            boost::bind(&NetworkSystem::udpHandleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    bool TCPInit()
    {
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "localhost", "4444");
        resolver.async_resolve(
            query,
            boost::bind(&NetworkSystem::tcpHandleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator)
        );
        tcpBusy = true; // While we're off asynchronously connecting we don't want to try sending anything
    }

    void TCPSend(TCPMessage &msg)
    {
        memcpy(tcpSendBuffer.c_array(), reinterpret_cast<uint8_t*>(&msg), sizeof(TCPMessage));
        tcpSocket.async_send(
            boost::asio::buffer(tcpSendBuffer),
            boost::bind(&NetworkSystem::tcpHandleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
        tcpBusy = true; // Will be turned off when the tcp message is sent
    }

    void TCPReceive()
    {
        tcpSocket.async_receive(
            boost::asio::buffer(tcpRecvBuffer),
            boost::bind(&NetworkSystem::tcpHandleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

private:
    void udpHandleResolve(const boost::system::error_code &error, udp::resolver::iterator endpointIter);
    void udpHandleReceive(const boost::system::error_code &error, std::size_t bytesTransferred);
    void udpHandleSend(const boost::system::error_code &error, std::size_t bytesTransferred);

    void tcpHandleResolve(const boost::system::error_code &error, tcp::resolver::iterator endpointIter);
    void tcpHandleConnect(const boost::system::error_code &error);
    void tcpHandleReceive(const boost::system::error_code &error, std::size_t bytesTransferred);
    void tcpHandleSend(const boost::system::error_code &error, std::size_t bytesTransferred);
        
    boost::array<uint8_t, sizeof(UDPMessage)> udpSendBuffer;
    boost::array<uint8_t, sizeof(UDPMessage)> udpRecvBuffer;
    SharedPtr<UDPMessage> udpRcvdMessagePtr;
    bool udpBusy;

    boost::array<uint8_t, sizeof(TCPMessage)> tcpSendBuffer;
    boost::array<uint8_t, sizeof(TCPMessage)> tcpRecvBuffer;
    SharedPtr<TCPMessage> tcpRcvdMessagePtr;
    bool tcpBusy;

    boost::asio::io_service io_service;    
    tcp::socket tcpSocket;
    tcp::resolver::iterator tcpEndpointIterator;
    udp::socket udpSocket;
    udp::endpoint udpEndpoint;

    // For storing received messages
    std::stack<UDPMessage> udpMessageStack;
    std::queue<TCPMessage> tcpMessageQueue;

    // For passing received messages from the async functions to the main thread
    Channel<UDPMessage> udpMessageChannel;
    Channel<TCPMessage> tcpMessageChannel;

    pThread ioServiceThread;
};
using pNetworkSystem = UniquePtr<NetworkSystem>;
