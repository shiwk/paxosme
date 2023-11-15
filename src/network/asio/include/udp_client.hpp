#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "network_client.hpp"

namespace paxosme
{
    class UdpClient : public NetworkClient, public std::enable_shared_from_this<UdpClient>
    {
    public:
        UdpClient(const EndPoint &);
        bool Send(const PodMsg &) override;
        static UdpClient *NewClient(const EndPoint &);
        ~UdpClient();

    private:
        paxosme::EndPoint endpoint_;
        // boost::asio::ip::udp::endpoint sender_endpoint_;
        boost::asio::io_service io_service_;
        boost::asio::ip::udp::socket socket_;
        std::future<void> handleLoop_;
    };
}

#endif