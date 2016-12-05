#include "NetworkSystem.hpp"

NetworkSystem::NetworkSystem()
    : udpSocket(io_service, udp::endpoint()) // Open our port on 4443
    , tcpSocket(io_service, tcp::endpoint())
    , myId(-1) // Wrap around max uint8
    , connectedAndIdentified(false)
    , snapshotReceivedThisFrame(false)
    , udpResolver(io_service)
    , tcpResolver(io_service)
{
    udpUpdateTimer = new boost::asio::deadline_timer(io_service, boost::posix_time::millisec(50));
}

NetworkSystem::~NetworkSystem()
{
    if (!shutdown)
    {
        std::cout << "Shutdown not called on NetworkSystem, calling now" << std::endl;
        Shutdown();
    }
}

void NetworkSystem::Start()
{
    shutdown = false;

    PlayerRecord fillerRecord;
    fillerRecord.id = -1;
    fillerRecord.transform.SetPosition(Vector3(0.f, 0.f, 0.f));
    playerRecords.fill(fillerRecord);
    oldPlayerRecords.fill(fillerRecord);
    activePlayers.fill(false);
    playerRecordHistory.fill({ false, static_cast<uint64_t>(std::time(nullptr)) });

    udpInit();
    tcpInit();
    ioServiceThread = MakeUnique<std::thread>(std::mem_fun(&NetworkSystem::ioServiceThreadFunc), this);
}

void NetworkSystem::Shutdown()
{
    shutdown = true;

    delete udpUpdateTimer;

    // Synchronously send a disconnect message to ensure it gets through before we stop the io_service
    TCPMessageData data;
    data.iAmDisconnectingData =
    {
        myId
    };
    TCPMessage msg =
    {
        TCPMessageType::IAmDisconnecting,
        static_cast<uint64_t>(std::time(nullptr)),
        data
    };
    boost::array<uint8_t, sizeof(TCPMessage)> buffer;
    memcpy(buffer.c_array(), reinterpret_cast<uint8_t*>(&msg), sizeof(TCPMessage));
    tcpSocket.send(boost::asio::buffer(buffer));

    udpUpdateTimer->cancel();

    io_service.stop();
    io_service.reset();
    ioServiceThread->join();
    tcpMessageChannel.Exit();
    udpMessageChannel.Exit();
}

void NetworkSystem::SetMyRecord(Transform & transform)
{
    Vector3 pos = transform.GetPosition();
    oldPlayerRecords[myId] = playerRecords[myId];
    playerRecords[myId].transform = transform; 
    playerRecords[myId].id = myId;
}

void NetworkSystem::udpInit()
{
    udp::resolver::query query(udp::v4(), "127.0.0.1", "4443");
    udpResolver.async_resolve(
        query,
        boost::bind(&NetworkSystem::udpHandleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator)
    );
}

void NetworkSystem::udpSend(UDPMessage &msg)
{
    memcpy(udpSendBuffer.c_array(), reinterpret_cast<uint8_t*>(&msg), sizeof(UDPMessage));
    udpSocket.async_send_to(
        boost::asio::buffer(udpSendBuffer),
        udpEndpoint,
        boost::bind(&NetworkSystem::udpHandleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

void NetworkSystem::udpReceive()
{
    udpSocket.async_receive(
        boost::asio::buffer(udpRecvBuffer),
        /*udpEndpoint,*/
        boost::bind(&NetworkSystem::udpHandleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

void NetworkSystem::udpUpdate()
{
    if (connectedAndIdentified)
    {
        UDPMessageData data;
        data.playerUpdateData =
        {
            playerRecords[myId],
            UDPMessageSender::Client
        };
        UDPMessage msg =
        {
            UDPMessageType::PlayerUpdate,
            static_cast<uint64_t>(std::time(nullptr)),
            data
        };
        udpSend(msg);
        std::cout << "UDP Update Sent" << std::endl;
    }
    udpUpdateTimer->async_wait(boost::bind(&NetworkSystem::udpUpdate, this)); // Call ourselves again
}

void NetworkSystem::tcpInit()
{
    tcp::resolver::query query(tcp::v4(), "127.0.0.1", "4443");
    tcpResolver.async_resolve(
        query,
        boost::bind(&NetworkSystem::tcpHandleResolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator)
    );
    tcpBusy = true; // Busy until we're connected
}

void NetworkSystem::tcpSend(TCPMessage &msg)
{
    memcpy(tcpSendBuffer.c_array(), reinterpret_cast<uint8_t*>(&msg), sizeof(TCPMessage));
    tcpSocket.async_send(
        boost::asio::buffer(tcpSendBuffer),
        boost::bind(&NetworkSystem::tcpHandleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

void NetworkSystem::tcpReceive()
{
    tcpSocket.async_receive(
        boost::asio::buffer(tcpRecvBuffer),
        boost::bind(&NetworkSystem::tcpHandleReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
}

void NetworkSystem::udpHandleResolve(const boost::system::error_code & error, udp::resolver::iterator endpointIter)
{
    if (!error)
    {
        // Take the first endpoint and set udpEndpoint
        udpEndpoint = *endpointIter;

        udpSocket = udp::socket(io_service, udp::endpoint(udp::v4(), 4444));
        //udpSocket.open(udp::v4()); // We need our own socket so we can listen to what the server wants to send us
        //udpSocket.bind(udpEndpoint);
        udpReceive(); // Start listening for messages
        //boost::asio::deadline_timer udpUpdateTimer(io_service, boost::posix_time::millisec(50));
        udpUpdateTimer->async_wait(boost::bind(&NetworkSystem::udpUpdate, this));
    }
    else
    {
        // Resolve failed for some reason
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        //abort();
#endif
    }
}

void NetworkSystem::udpHandleReceive(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        assert(bytesTransferred == UDPMessageSize);
        // There should be a new message in the udpRecvBuffer
        UDPMessage *recvdMsg = reinterpret_cast<UDPMessage*>(udpRecvBuffer.c_array());
        // Send it down the message channel to be handled by the main loop
        udpMessageChannel.Write(*recvdMsg);
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        //abort();
#endif
    }
    udpReceive(); // Back to listening
}

void NetworkSystem::udpHandleSend(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        std::cout << "UDP Message sent! " << std::endl;
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
    }
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
        //abort();
#endif
    }
}

void NetworkSystem::tcpHandleConnect(const boost::system::error_code & error)
{
    if (!error)
    {
        // The connection was successful!
        tcpBusy = false; // We can now send stuff!        
        // Start receiving messages too!
        tcpReceive();
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
        //abort();
#endif
    }
}

void NetworkSystem::tcpHandleReceive(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        assert(bytesTransferred == TCPMessageSize);
        // There should be a new message in the tcpRecvBuffer
        TCPMessage *recvdMsg = reinterpret_cast<TCPMessage*>(tcpRecvBuffer.c_array());
        // Send it down the message channel to be handled by the main loop
        tcpMessageChannel.Write(*recvdMsg);
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
#ifdef _DEBUG
        //abort();
#endif
    }
    tcpReceive();
}

void NetworkSystem::tcpHandleSend(const boost::system::error_code & error, std::size_t bytesTransferred)
{
    if (!error)
    {
        std::cout << "TCP Message sent! " << std::endl;
    }
    else
    {
        std::cout << "Error: " << error.message() << std::endl;
    }
}

void NetworkSystem::Tick()
{
    for (auto &record : playerRecordHistory)
    {
        record.UpdatedThisFrame = false;
    }
    snapshotReceivedThisFrame = false;

    while (!tcpMessageChannel.Empty())
    {
        TCPMessage msg = tcpMessageChannel.Read();   
        switch (msg.type)
        {
        case TCPMessageType::YouAreConnected:
        {
            TCPMessageYouAreConnectedData data = msg.data.youAreConnectedData;
            myId = data.id;
            activePlayers[myId] = true; // We are an active player
            connectedAndIdentified = true;
#ifdef _DEBUG
            std::cout << "MyID: " << static_cast<char>(myId + 48) << std::endl;
#endif
            {
                // Send an IWantToConnect message to the server now that we have an id
                TCPMessageData data;
                data.ipv4ConnectData =
                {
                    myId,
                    "127.0.0.1",
                    "4444"
                };
                TCPMessage msg =
                {
                    TCPMessageType::IWantToConnectIPv4,
                    static_cast<uint64_t>(std::time(nullptr)),
                    data
                };
                tcpSend(msg);
            }
            break;
        }
        case TCPMessageType::ConnectTell:
        {
            PlayerRecord newPlayer = msg.data.connectTellData.newPlayer;
            playerRecords[newPlayer.id] = newPlayer;
            oldPlayerRecords[newPlayer.id] = newPlayer; // No old data, but we need something
            activePlayers[newPlayer.id] = true;
            playerRecordHistory[newPlayer.id].UpdatedThisFrame = true;
            playerRecordHistory[newPlayer.id].timestamp = msg.unixTimestamp;
#ifdef _DEBUG
            std::cout << "New Player connected as ID: " << static_cast<char>(newPlayer.id + 48) << std::endl;
#endif
            break;
        }
        case TCPMessageType::DisconnectTell:
        {
            uint8_t disconnectedPlayer = msg.data.disconnectTellData.id;
            activePlayers[disconnectedPlayer] = false;
#ifdef _DEBUG
            std::cout << "ID: " << static_cast<char>(disconnectedPlayer + 48) << "Disconnected" << std::endl;
#endif
            break;
        }
        case TCPMessageType::Snapshot:
        {
            TCPMessageSnapshotData snapshot = msg.data.snapshotData;
            oldPlayerRecords = playerRecords; // Current data is now old
            memcpy(playerRecords.data(), snapshot.records, sizeof(snapshot.records)); // Using lots of mem functions in this program
            for (auto &record : playerRecordHistory)
            {
                record.timestamp = msg.unixTimestamp;
                record.UpdatedThisFrame = true;
            }
            snapshotReceivedThisFrame = true;
#ifdef _DEBUG
            std::cout << "New snapshot received!" << std::endl;
#endif
            break;
        }
        case TCPMessageType::Ping:
        {
            // Respond with pong
            TCPMessage response =
            {
                TCPMessageType::Pong,
                static_cast<uint64_t>(std::time(nullptr)),
                {}
            };
            tcpSend(response);
            break;
        }
        case TCPMessageType::IAmDisconnecting: // On the off chance the server is shutdown?
        {
            std::cout << "Server communicated it is disconnecting, will not receive anymore updates\nWe're on our own now." << std::endl;
            activePlayers.fill(false);
            activePlayers[myId] = true;
            connectedAndIdentified = false;
            break;
        }
        default:
            std::cout << "Unrecognised TCP Message Type!" << std::endl;
            break;
        }
    }

    while (!udpMessageChannel.Empty())
    {
        UDPMessage msg = udpMessageChannel.Read();
        switch (msg.type)
        {
        case UDPMessageType::PlayerUpdate: // New data about another client
        case UDPMessageType::ActuallyUpdate: // Correct the client on their own data
        {
            if (msg.data.playerUpdateData.sender == UDPMessageSender::Client) break; // We're talking to ourself 
            PlayerRecord &newRecord = msg.data.actuallyUpdateData.playerData;
            if (playerRecordHistory[newRecord.id].UpdatedThisFrame)
            {
                if (playerRecordHistory[newRecord.id].timestamp > msg.unixTimestamp) // Stored record is newer than the one in the message
                {
                    break;
                }
                // Else the one we're looking at is newer so let's store it
            }
            oldPlayerRecords[newRecord.id] = playerRecords[newRecord.id]; // Save the old record
            playerRecords[newRecord.id] = newRecord; // Store the new record
            playerRecordHistory[newRecord.id].UpdatedThisFrame = true;
            playerRecordHistory[newRecord.id].timestamp = msg.unixTimestamp;
#ifdef _DEBUG
            std::cout << "New Data for Player ID: " << static_cast<char>(newRecord.id + 48) << std::endl;
#endif
            break;
        }
        case UDPMessageType::StillThere:
        {
            // Respond with a StillHere message
            UDPMessageData data;
            data.stillHereData =
            {
                myId
            };
            UDPMessage response =
            {
                UDPMessageType::StillHere,
                static_cast<uint64_t>(std::time(nullptr)),
                data
            };
            udpSend(response);
            break;
        }
        default:
            std::cout << "Unrecognised UDP Message Type!" << std::endl;
            break;
        }
    }
}
