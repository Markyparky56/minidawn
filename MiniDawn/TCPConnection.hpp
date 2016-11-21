#pragma once
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:
    typedef boost::shared_ptr<TCPConnection> ptr;

    static ptr Create(boost::asio::io_service &io_service)
    {
        return ptr(new TCPConnection(io_service));
    }

    tcp::socket &Socket()
    {
        return socket;
    }

    void Start()
    {
        message = "";

        boost::asio::async_write(socket, boost::asio::buffer(message), boost::bind(&TCPConnection::HandleWrite, shared_from_this()));
    }

private:
    TCPConnection(boost::asio::io_service &io_service)
        : socket(io_service)
    {
    }

    // Called when the async_write function is completed
    void HandleWrite(const boost::system::error_code &error, std::size_t bytes_transferred)
    {
    }

    tcp::socket socket;
    std::string message;
};
