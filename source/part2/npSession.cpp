#include "header/npSession.h"
#include "header/console.h"

sessionToNP::sessionToNP(std::shared_ptr<boost::asio::ip::tcp::socket> originalSocket, tcp::socket socket_, int index)
    : socket_(std::move(socket_))
{
    currentSocket = originalSocket;
    cerr << "shell " << index << " is creating session" << endl;
    id = index;
    Receive = "";
    filename = "./test_case/" + shells[id].file;
    inputFile.open(filename, ios::in);
}

void sessionToNP::start()
{
    do_read();
}

void sessionToNP::do_read()
{
    cerr << "shell " << id << " start do read" << endl;
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    Receive += data_;
                                    memset(data_, 0, length);
                                    size_t pos;
                                    if ((pos = Receive.find("%")) != string::npos)
                                    {
                                        // cout << Receive << endl;
                                        string returnBody = output_shell(id, Receive);
                                        currentSocket->send(boost::asio::buffer(returnBody, returnBody.size()));
                                        Receive = "";
                                        do_write();
                                    }
                                    cerr << "shell " << id << " finish async_read_some" << endl;
                                    do_read();
                                }
                                else
                                    // perror("async_read");
                                    socket_.close();
                            });
}

void sessionToNP::do_write()
{
    cerr << "shell " << id << " start do write" << endl;
    auto self(shared_from_this());
    string input;
    // if(file_input.eof()) return;
    if (!getline(inputFile, input))
    {
        cerr << "getline fail" << endl;
    }
    input = input + "\n";
    string returnBody = output_command(id, input);
    currentSocket->send(boost::asio::buffer(returnBody, returnBody.size()));
    socket_.send(boost::asio::buffer(input.c_str(), input.length()));
}