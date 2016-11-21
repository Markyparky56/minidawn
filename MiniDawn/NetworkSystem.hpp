#pragma once
/***************************************************************************************************
    A Networking System to manage a UDP and TCP connection to a remote host which will have
    authorative control over the true locations of objects in the scene.
    Will, on start up, connect via TCP to the server and syncronise with any other connected clients
    Will then regularly (e.g. 1/20th of a second) update the server of the local users current position
    and direction (any any other important data pertaining to the user which is can fit in the pakcet)
    Will listen for UDP packets from the server (also sent regularly) for authorative updates which it 
    may need use to correct local data (to the best of it's ability, minimising unrealistic movements
    which may cause rubberbanding or breaking of reality)
    Will listen on the original TCP connection for important messages from the server, like new clients
    connecting or disconnecting, snapshots of the (global) world (as opposed to local changes).
***************************************************************************************************/

#include "UniquePtr.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class NetworkSystem : public boost::enable_shared_from_this<NetworkSystem>
{
public:
    NetworkSystem();
    ~NetworkSystem();

    void UDPSend()
    {
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), "localhost", "4444");
        resolver.async_resolve(query, boost::bind(&NetworkSystem::UDPEndpointResolverHandler, shared_from_this()));
        udp::socket socket(io_service);
        socket.open(udp::v4());
    }

private:
    void UDPEndpointResolverHandler(const boost::system::error_code &error_code, boost::asio::ip::tcp::resolver::iterator iterator)
    {

    }

    udp::resolver udpResolver;
    tcp::resolver tcpResolver;
    tcp::socket tcpSocket;
    udp::socket udpSocket;
};
using pNetworkSystem = UniquePtr<NetworkSystem>;
