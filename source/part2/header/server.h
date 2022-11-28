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
  ~server();

private:
  void do_accept();
  
  tcp::acceptor acceptor_;
};
extern server *s;

#endif