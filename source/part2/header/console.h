#if !defined(__CONSOLE_H__)
  #define __CONSOLE_H__
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind/bind.hpp>
using boost::asio::io_service;
using boost::asio::ip::tcp;
using namespace std;

struct npshell
{
  string host;
  string port;
  string file;
};

string print_header();
string print_endHeader();
string printHTML();
string output_command(int session, string content);
string output_shell(int session, string content);
vector<string> SeperateQueryString(string QS);
void buildShells(string QS);

extern vector<npshell> shells;

#endif