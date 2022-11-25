#if !defined(__SERVER_SESSION_H__)
#define __SERVER_SESSION_H__
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <sstream>
#include <string.h>
#include <boost/asio.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <io.h>
using boost::asio::io_service;
using boost::asio::ip::tcp;
using namespace std;

extern boost::asio::io_context io_context;

class server
{
public:
  server(boost::asio::io_context &io_context, short port);
  void do_tcpConnect(string queryString,tcp::socket socket);

private:
  void do_accept();
  void connection(const int i, const boost::system::error_code &err, const tcp::resolver::iterator it);
  void create_session(const int i, const boost::system::error_code &err, const tcp::resolver::iterator it);
  std::shared_ptr<boost::asio::ip::tcp::socket> currentSocket;
  tcp::acceptor acceptor_;
  tcp::socket *socket_[5];
  tcp::resolver resolve;
};
extern server *s;

#endif