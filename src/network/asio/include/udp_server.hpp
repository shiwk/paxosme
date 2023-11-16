#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "network_server.hpp"
#include <boost/asio.hpp>

namespace paxosme {
    class UdpServer : public NetworkServer {
    public:
        UdpServer(const Peer &);
        void Start(Network::MsgCallback) override;

        void Shutdown() override;

        bool Running() override;

        ~UdpServer() override;

    private:
        int socket_fd_;
        bool is_running_ = false;
        boost::asio::ip::udp::endpoint sender_endpoint_;
        boost::asio::io_service io_service_;
        boost::asio::ip::udp::socket socket_;
        enum { max_length = 1024 * 1024};
        char data_[max_length];
        std::mutex mutex_;
        std::future<void> handleLoop_;
        void DoReceive();
        void DoSend(std::size_t length);
    };
}

#endif