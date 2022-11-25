#include "header/panel.h"
#include "header/console.h"
#include "header/server.h"
#include "header/htmlSession.h"
#include "header/npSession.h"
boost::asio::io_context io_context;

server::server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), resolve(io_context)
{
    do_accept();
}

void server::do_accept()
{
    cout<<"ready to accept"<<endl;
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<session>(std::move(socket))->start();
            }
            cout<<"acceptor_"<<endl;
            do_accept();
        });
}

void server::do_tcpConnect(string queryString,tcp::socket socket)
{
    currentSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));
    for (size_t i = 0; i < shells.size(); i++)
    {
        // cerr << "create query" << endl;
        tcp::resolver::query query(shells[i].host, shells[i].port);
        resolve.async_resolve(query,
                              boost::bind(&server::connection, this, i, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    }
}

void server::connection(const int i, const boost::system::error_code &err, const tcp::resolver::iterator it)
{
    if (!err)
    {
        // cerr << "Shell " << i << " is prepared to connect" << endl;
        socket_[i] = new tcp::socket(io_context);
        (*socket_[i]).async_connect(*it, boost::bind(&server::create_session, this, i, boost::asio::placeholders::error, it));
    }
    else{
        cerr<<"Shell " << i << " is not prepared to connect" << endl;
    }
}

void server::create_session(const int i, const boost::system::error_code &err, const tcp::resolver::iterator it)
{
    if (!err)
    {
        // cerr << "Shell " << i << " is connected to " << shells[i].host << ":" << shells[i].port << endl;
        std::make_shared<sessionToNP>(currentSocket, std::move(*socket_[i]), i)->start();
    }
    else{
        cerr<<"Shell " << i << " is not connected"<<endl;
    }
}