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
#include <boost/date_time/posix_time/posix_time.hpp> // For async timers
#include "Channel.hpp"
#include "Protocol.hpp"
#include <thread>
#include <functional>
#include <array>

using pThread = UniquePtr<std::thread>;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class NetworkSystem : public boost::enable_shared_from_this<NetworkSystem>
{
public:
    struct PlayerRecordHistory // Name is maybe a little ambiguous, tracks the last time a record was updated, and if it has been updated this frame
    {
        bool UpdatedThisFrame;
        uint64_t timestamp;
    };

public:
    NetworkSystem();
    ~NetworkSystem();

    void Start();
    void Shutdown();

    const std::array<PlayerRecord, 16> GetPlayerRecords() { return playerRecords; }
    const std::array<PlayerRecord, 16> GetOldPlayerRecords() { return oldPlayerRecords; }
    const std::array<PlayerRecordHistory, 16> GetPlayerRecordHistory() { return playerRecordHistory; }
    const std::array<bool, 16> GetActivePlayers() { return activePlayers; }
    const uint8_t GetMyId() { return myId; }
    const bool ConnectedAndIdentified() { return connectedAndIdentified; }
    const bool SnapshotReceivedThisFrame() { return snapshotReceivedThisFrame; }
    void SetMyRecord(Transform &transform);
    void Tick();

private:
    void ioServiceThreadFunc()
    {
        std::cout << "io_service thread starting...\n";
        size_t ret = io_service.run();
        std::cout << ret << " handlers executed by io_service during run." << std::endl;
    }

    void udpInit();
    void udpSend(UDPMessage &msg);
    void udpReceive();
    void udpUpdate(); // Send a PlayerUpdate message to the server

    void tcpInit();
    void tcpSend(TCPMessage &msg);
    void tcpReceive();

    void udpHandleResolve(const boost::system::error_code &error, udp::resolver::iterator endpointIter);
    void udpHandleReceive(const boost::system::error_code &error, std::size_t bytesTransferred);
    void udpHandleSend(const boost::system::error_code &error, std::size_t bytesTransferred);

    void tcpHandleResolve(const boost::system::error_code &error, tcp::resolver::iterator endpointIter);
    void tcpHandleConnect(const boost::system::error_code &error);
    void tcpHandleReceive(const boost::system::error_code &error, std::size_t bytesTransferred);
    void tcpHandleSend(const boost::system::error_code &error, std::size_t bytesTransferred);
        

    std::array<PlayerRecord, 16> playerRecords;
    std::array<PlayerRecord, 16> oldPlayerRecords;
    uint8_t myId;
    std::array<bool, 16> activePlayers;
    std::array<PlayerRecordHistory, 16> playerRecordHistory;

    boost::array<uint8_t, sizeof(UDPMessage)> udpSendBuffer;
    boost::array<uint8_t, sizeof(UDPMessage)> udpRecvBuffer;

    boost::array<uint8_t, sizeof(TCPMessage)> tcpSendBuffer;
    boost::array<uint8_t, sizeof(TCPMessage)> tcpRecvBuffer;
    bool tcpBusy;

    boost::asio::io_service io_service;    
    tcp::socket tcpSocket;
    tcp::resolver tcpResolver;
    tcp::resolver::iterator tcpEndpointIterator;
    udp::socket udpSocket;
    udp::resolver udpResolver;
    udp::endpoint udpEndpoint;
    boost::asio::deadline_timer *udpUpdateTimer;

    // For passing received messages from the async functions to the main thread
    Channel<UDPMessage, std::stack<UDPMessage> > udpMessageChannel;
    Channel<TCPMessage, std::queue<TCPMessage> > tcpMessageChannel;

    pThread ioServiceThread;
    bool shutdown;
    bool connectedAndIdentified;
    bool snapshotReceivedThisFrame;
};
using pNetworkSystem = UniquePtr<NetworkSystem>;
