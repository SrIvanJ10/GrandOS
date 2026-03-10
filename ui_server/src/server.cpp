#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>

#include "server.hpp"

server::server(io_context& ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(ioc, endpoint) {}

void server::run() {
    do_accept();
}

void server::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp_socket socket) {
            if (!ec) {
                std::make_shared<session>(std::move(socket))->run();
            }
            do_accept();
        }
    );
}

