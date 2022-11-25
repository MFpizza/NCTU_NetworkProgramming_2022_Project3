#if !defined(__NP_SESSION_H__)
    #define __NP_SESSION_H__

#include "console.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string.h>
using boost::asio::io_service;
using boost::asio::ip::tcp;
using namespace std;

class sessionToNP
    : public std::enable_shared_from_this<sessionToNP>
{
public:
  sessionToNP(std::shared_ptr<boost::asio::ip::tcp::socket> originalSocket, tcp::socket socket_, int index);
  void start();

private:
  void do_read();
  void do_write();

  tcp::socket socket_;
  enum
  {
    max_length = 1024
  };
  char data_[max_length];
  ifstream inputFile;
  int id;
  string filename;
  string Receive;
  string status_str = "HTTP/1.1 200 OK\n";
  std::shared_ptr<boost::asio::ip::tcp::socket> currentSocket;
};

#endif