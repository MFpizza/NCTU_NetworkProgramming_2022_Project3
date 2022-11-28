#include "header/htmlSession.h"
#include "header/server.h"
#include "header/console.h"
#include "header/npSession.h"
session::session(tcp::socket socket)
    : socket_(std::move(socket))
{
}

void session::start()
{
    do_read();
}

void session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    //* REQUEST_METHOD, REQUEST_URI, SERVER_PROTOCOL, HTTP_HOST
                                    char REQUEST_METHOD[100];
                                    char REQUEST_URI[1024];
                                    char SERVER_PROTOCOL[100];
                                    char HTTP_HOST[100];
                                    sscanf(data_, "%s %s %s\n\rHost: %s\n",
                                           REQUEST_METHOD, REQUEST_URI, SERVER_PROTOCOL, HTTP_HOST);

                                    //* SERVER_ADDR, SERVER_PORT, REMOTE_ADDR, REMOTE_PORT
                                    char SERVER_ADDR[100];
                                    char SERVER_PORT[10];
                                    char REMOTE_ADDR[100];
                                    char REMOTE_PORT[10];
                                    strcpy(SERVER_ADDR,
                                           socket_.local_endpoint().address().to_string().c_str());
                                    sprintf(SERVER_PORT, "%u", socket_.local_endpoint().port());
                                    strcpy(REMOTE_ADDR,
                                           socket_.remote_endpoint().address().to_string().c_str());
                                    sprintf(REMOTE_PORT, "%u", socket_.remote_endpoint().port());
                                    //* QUERY STRING
                                    char EXEC_FILE[100] = "\0";
                                    char QUERY_STRING[1024] = "\0";
                                    sscanf(REQUEST_URI, "%[^?]?%s", EXEC_FILE, QUERY_STRING);
                                    EXEC_FILE[strlen(EXEC_FILE)] = '\0';
                                    QUERY_STRING[strlen(QUERY_STRING)] = '\0';
                                    target_uri = std::string(EXEC_FILE);
                                    // cout << "\nRU: " << REQUEST_URI << endl;
                                    // cout << "EXEC: " << EXEC_FILE << endl;
                                    // cout << "QS: " << QUERY_STRING << endl;
                                    // cout << "TU: " << target_uri << endl;
                                    queryString = string(QUERY_STRING);

                                    if (target_uri == "/panel.cgi")
                                        do_panel();
                                    else if (target_uri == "/console.cgi")
                                        do_write(length);
                                    else
                                        exit(0);
                                }
                            });
}
void session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    //* important
    socket_.async_send(boost::asio::buffer(status_str, strlen(status_str)),
                       [this, self](boost::system::error_code ec, std::size_t /*length*/)
                       {
                           if (!ec)
                           {
                               // cout<<data_<<endl;
                               //    cout << "do write" << endl;
                           }
                           else
                           {
                               cout << "ERROR!\n";
                               cout << ec << endl;
                           }
                       });
    buildShells(queryString);
    string returnBody = printHTML();
    socket_.async_send(boost::asio::buffer(returnBody, returnBody.size()),
                       [this, self](boost::system::error_code ec, std::size_t /*length*/)
                       {
                           if (!ec)
                           {
                               std::shared_ptr<boost::asio::ip::tcp::socket> currentSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket_));
                               for (size_t i = 0; i < shells.size(); i++)
                               {
                                    tcp::socket socketToNp(io_context);
                                    std::make_shared<sessionToNP>(currentSocket, std::move(socketToNp), i)->start();
    
                                   // cerr << "create query" << endl;
                                   
                               }
                               // cout<<data_<<endl;

                               //    cout << "finish send output_shell" << endl;
                           }
                           else
                           {
                               cout << "ERROR!\n";
                               cout << ec << endl;
                           }
                       });
}

void session::do_panel()
{
    auto self(shared_from_this());
    //* important
    socket_.async_send(boost::asio::buffer(status_str, strlen(status_str)),
                       [this, self](boost::system::error_code ec, std::size_t /*length*/)
                       {
                           if (!ec)
                           {

                               //    cout << "finish header pass" << endl;
                           }
                           else
                           {
                               cout << "ERROR!\n";
                               cout << ec << endl;
                           }
                       });
    cout << "do_panel_______________________________________" << endl;
    string returnBody = panelHTML();
    socket_.async_send(boost::asio::buffer(returnBody.c_str(), returnBody.size()),
                       [this, self](boost::system::error_code ec, std::size_t /*length*/)
                       {
                           if (!ec)
                           {
                               //    cout << "finish write panel" << endl;

                               do_read();
                           }
                           else
                           {
                               cout << "ERROR!\n";
                               cout << ec << endl;
                           }
                       });
}