#include "udp_server.hpp"
#include <glog/logging.h>

using boost::asio::ip::udp;

paxosme::UdpServer::UdpServer(const EndPoint &peer) : socket_(boost::asio::ip::udp::socket(io_service_, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), peer.port)))
{
}

void paxosme::UdpServer::Start(Network::MsgCallback)
{

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

// class server
// {
// public:
//   server(boost::asio::io_service &io_service, short port)
//       : socket_(io_service, udp::endpoint(udp::v4(), port))
//   {
//     do_receive();
//   }

//   void do_receive()
//   {
//     socket_.async_receive_from(
//         boost::asio::buffer(data_, max_length), sender_endpoint_,
//         [this](boost::system::error_code ec, std::size_t bytes_recvd)
//         {
//           if (!ec && bytes_recvd > 0)
//           {
//             do_send(bytes_recvd);
//           }
//           else
//           {
//             do_receive();
//           }
//         });
//   }

//   void do_send(std::size_t length)
//   {
//     socket_.async_send_to(
//         boost::asio::buffer(data_, length), sender_endpoint_,
//         [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
//         {
//           do_receive();
//         });
//   }

// private:
//   udp::socket socket_;
//   udp::endpoint sender_endpoint_;
//   enum
//   {
//     max_length = 1024
//   };
//   char data_[max_length];
// };

// int main(int argc, char *argv[])
// {
//   try
//   {
//     if (argc != 2)
//     {
//       std::cerr << "Usage: async_udp_echo_server <port>\n";
//       return 1;
//     }

//     boost::asio::io_service io_service;

//     server s(io_service, std::atoi(argv[1]));

//     io_service.run();
//   }
//   catch (std::exception &e)
//   {
//     std::cerr << "Exception: " << e.what() << "\n";
//   }

//   return 0;
// }