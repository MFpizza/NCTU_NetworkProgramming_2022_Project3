#include "header/server.h"
#include <boost/asio/signal_set.hpp>
#include <iostream>
void handler(const boost::system::error_code &error, int signal_number)
{
    std::cout << "handling signal " << signal_number << std::endl;
    exit(1);
}

server *s;
int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }
        boost::asio::signal_set signals(io_context, SIGINT);

        // Start an asynchronous wait for one of the signals to occur.
        signals.async_wait(handler);

        cout << "Port: " << std::atoi(argv[1]) << endl;
        s = new server(io_context, std::atoi(argv[1]));

        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}