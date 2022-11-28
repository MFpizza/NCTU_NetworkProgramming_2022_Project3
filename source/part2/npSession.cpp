#include "header/npSession.h"
#include "header/console.h"

sessionToNP::sessionToNP(std::shared_ptr<boost::asio::ip::tcp::socket> originalSocket, tcp::socket socket_, int index)
    : socket_(std::move(socket_))
{
    currentSocket = originalSocket;
    // cerr << "shell " << index << " is creating session" << endl;
    id = index;
    Receive = "";
    memset(data_, 0, max_length);
    filename = "./test_case/" + shells[id].file;
    inputFile.open(filename, ios::in);
}
sessionToNP::~sessionToNP(){
     cerr << "shell " << id << " is closing session" << endl;
}

void sessionToNP::start()
{
    do_read();
}

void sessionToNP::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    // printf("%s",data_);
                                    // fflush(stdout);
                                    Receive += string(data_);
                                    memset(data_, 0, max_length);
                                    size_t pos;
                                    if ((pos = Receive.find("% ")) != string::npos)
                                    {
                                        string returnBody = output_shell(id, Receive);
                                        // cout<<"--------------------------------"<<endl;
                                        // cout << Receive << endl;
                                        // cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
                                        // cout << returnBody << endl;
                                        // boost::asio::async_write(*currentSocket, boost::asio::buffer(returnBody, returnBody.size()),
                                        //                          [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                        //                          {
                                        //                              if (!ec)
                                        //                              {
                                        //                                 cout<<"async_write finished"<<endl;
                                        //                              }
                                        //                              else
                                        //                              {
                                        //                                  perror("async_write");
                                        //                              }
                                        //                          });
                                        currentSocket->send(boost::asio::buffer(returnBody, returnBody.size()));
                                        Receive = "";
                                        do_write();
                                    }
                                    do_read();
                                }
                                else
                                {
                                    perror("async_read");
                                    socket_.close();
                                }
                            });
}

void sessionToNP::do_write()
{
    auto self(shared_from_this());
    string input;
    // if(file_input.eof()) return;
    if (!getline(inputFile, input))
    {
        cerr << "getline fail" << endl;
    }
    input = input + "\n";
    string returnBody = output_command(id, input);
    boost::asio::async_write(*currentSocket, boost::asio::buffer(returnBody, returnBody.size()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                 }
                                 else
                                 {
                                     perror("async_write");
                                 }
                             });
    boost::asio::async_write(socket_, boost::asio::buffer(input.c_str(), input.length()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                 }
                                 else
                                 {
                                     perror("async_write");
                                 }
                             });
}