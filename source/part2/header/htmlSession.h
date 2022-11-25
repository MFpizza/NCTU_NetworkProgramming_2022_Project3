#if !defined(__HTML_SESSION_H__)
    #define __HTML_SESSION_H__
    
#include "panel.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string.h>
using boost::asio::io_service;
using boost::asio::ip::tcp;
using std::cerr;
using std::cout;
using std::endl;
using std::string;

class session
    : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket);
  void start();

private:
  void do_read();
  void do_write(std::size_t length);
  void do_panel();

  tcp::socket socket_;
  enum
  {
    max_length = 1024
  };
  char status_str[200] = "HTTP/1.1 200 OK\n";
  char data_[max_length];
  std::string target_uri;
  std::string queryString;
};
#endif