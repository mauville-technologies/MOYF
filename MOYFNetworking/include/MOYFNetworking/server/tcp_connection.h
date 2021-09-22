//
// Created by ozzadar on 2021-08-22.
//

#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace MOYF {
    using boost::asio::ip::tcp;
    namespace io = boost::asio;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
    public:
        using pointer = std::shared_ptr<TCPConnection>;

        static pointer Create(io::ip::tcp::socket&& socket) {
            return pointer(new TCPConnection(std::move(socket)));
        }

        inline const std::string& GetUsername() const { return _username; }

        tcp::socket& Socket() {
            return _socket;
        }

        void Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
        void Post(const std::string& message);

    private:
        explicit TCPConnection(io::ip::tcp::socket&& socket);

        // Wait for a new message from client
        void asyncRead();
        void onRead(boost::system::error_code ec, size_t bytesTranferred);

        void asyncWrite();
        void onWrite(boost::system::error_code ec, size_t bytesTransferred);

    private:
        tcp::socket _socket;
        std::string _username;

        std::queue<std::string> _outgoingMessages;
        io::streambuf _streamBuf {65536};

        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    };
}


