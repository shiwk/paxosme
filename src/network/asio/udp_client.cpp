//
// daytime_client.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>
#include "udp_client.hpp"
#include <glog/logging.h>
using boost::asio::ip::udp;


paxosme::UdpClient::UdpClient(const paxosme::EndPoint &endpoint) : endpoint_(endpoint), socket_(std::move(boost::asio::ip::udp::socket(io_service_, udp::v4())))
{
    handleLoop_ = std::move(std::async(std::launch::async, [this]()
                                       {
                                            // Class to inform the io_context when it has work to do.
                                            boost::asio::io_service::work work(io_service_);
                                            io_service_.run();
                                            DLOG(INFO) << "start client... "; 
                                        }));
}

paxosme::UdpClient::~UdpClient()
{
    io_service_.stop();
}

bool paxosme::UdpClient::Send(const PodMsg &podMsg)
{
    try
    {
        udp::resolver resolver(io_service_);

        auto iter =
            resolver.async_resolve(
                {endpoint_.host, std::to_string(endpoint_.port)},
                boost::asio::use_future);
        DLOG(INFO) << "before send... ";

        // The async_resolve operation above returns the endpoint iterator as a
        // future value that is not retrieved ...

        udp::socket socket(io_service_, udp::v4());

        // std::array<char, 1> send_buf = {{0}};
        std::future<std::size_t> send_length =
            socket.async_send_to(boost::asio::buffer(podMsg.proposedLogValue), // TODO I: podmsg to buffer
                                 *iter.get(), // ... until here. This call may block.
                                 boost::asio::use_future);

        DLOG(INFO) << "send... ";

        // Do other things here while the send completes.

        size_t send_size = send_length.get(); // Blocks until the send is complete. Throws any errors.
        DLOG(INFO) << "send_size = " << send_size << std::endl;

        std::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        std::future<std::size_t> recv_length =
            socket.async_receive_from(
                boost::asio::buffer(recv_buf),
                sender_endpoint,
                boost::asio::use_future);

        // Do other things here while the receive completes.

        size_t recv_size = recv_length.get();
        DLOG(INFO) << "recv_size = " << recv_size << std::endl;
        DLOG(INFO) << "recv_buf.data() = " << recv_buf.data() << std::endl;


        return true;
    }
    catch (std::system_error &e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

paxosme::UdpClient *paxosme::UdpClient::NewClient(const paxosme::EndPoint &endpoint)
{
    return new paxosme::UdpClient(endpoint);
}

paxosme::NetworkClient *paxosme::NetworkClient::New(const paxosme::Peer &peer)
{
    return new paxosme::UdpClient(paxosme::EndPoint{peer.ip, peer.port});
}
