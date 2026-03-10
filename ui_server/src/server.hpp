#ifndef UI_SERVER_SERVER_HPP
#define UI_SERVER_SERVER_HPP

//libraries
    #include <boost/asio.hpp>
    #include <boost/beast.hpp>
    #include <memory>

//project headers
    #include "session.hpp"

//namespaces & aliases
    namespace asio = boost::asio;
    namespace beast = boost::beast;

    using io_context = asio::io_context;
    using tcp = asio::ip::tcp;
    using acceptor = tcp::acceptor;
    using tcp_socket = tcp::socket;



class server {
    public:
        explicit server(io_context& ioc, tcp::endpoint endpoint);
        void run();

    private:
        io_context& ioc_;
        acceptor acceptor_;
        void do_accept();
};

#endif