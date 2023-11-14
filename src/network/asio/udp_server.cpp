#include "udp_server.hpp"
#include <glog/logging.h>

using boost::asio::ip::udp;

paxosme::UdpServer::UdpServer(const EndPoint &peer) : socket_(boost::asio::ip::udp::socket(io_service_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), peer.port)))
{
}

void paxosme::UdpServer::Start(const Network::MsgCallback &msgCallBack)
{

    msgCallback_ = std::move(msgCallBack);
    handleLoop_ = std::move(std::async(std::launch::async, [this]()
                                       {
            LOG(INFO) << "Start: DoReceive";
            DoReceive();
            io_service_.run(); }));
    is_running_ = true;
}

void paxosme::UdpServer::Shutdown()
{
    std::unique_lock<std::mutex> lock(mutex_);
    if (!is_running_)
    {
        return;
    }
    io_service_.stop();
    is_running_ = false;
}

bool paxosme::UdpServer::Running()
{
    std::unique_lock<std::mutex> lock(mutex_);
    return is_running_;
}

paxosme::UdpServer::~UdpServer()
{
    if (Running())
    {
        Shutdown();
    }
}

void paxosme::UdpServer::DoReceive()
{
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd)
        {
            if (!ec && bytes_recvd > 0)
            {
                msgCallback_(std::string(data_, bytes_recvd));
                DoSend(bytes_recvd);
            }
            else
            {
                DoReceive();
            }
        });
}

void paxosme::UdpServer::DoSend(std::size_t length)
{
    socket_.async_send_to(
        boost::asio::buffer(data_, length), sender_endpoint_,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
        {
            DoReceive();
        });
}

paxosme::NetworkServer *paxosme::NetworkServer::New(const EndPoint &peer)
{
    return new paxosme::UdpServer(peer);
}