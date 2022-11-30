#include "header/panel.h"
#include "header/console.h"
#include "header/server.h"
#include "header/htmlSession.h"
#include "header/npSession.h"
boost::asio::io_context io_context;

server::server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    do_accept();
}
server::~server(){
    cout<<"server is closed"<<endl;
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

void server::stop(){
    acceptor_.close();
}