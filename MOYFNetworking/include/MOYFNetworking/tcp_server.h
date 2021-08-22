//
// Created by ozzadar on 2021-08-22.
//

#pragma once

#include <boost/asio.hpp>
#include <MOYFNetworking/tcp_connection.h>
#include <functional>

namespace MOYF {
    enum class IPV {
        V4,
        V6
    };

    class TCPServer {

    public:
        TCPServer(IPV ipv, int port);

        int Run();

        template <typename T>
        void WriteToConnection(int connectionIndex, const T& message);

        template<typename T>
        using ListenCallback = std::function<void(int, const T&)>;

        template <typename T>
        void RegisterListenCallback(ListenCallback<T> callback);

    private:
        void startAccept();
    private:
        IPV _ipVersion;
        int _port;

        boost::asio::io_context _ioContext;
        boost::asio::ip::tcp::acceptor  _acceptor;

        std::vector<TCPConnection::pointer> _connections {};
    };
}


