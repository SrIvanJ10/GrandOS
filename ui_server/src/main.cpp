#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <memory>

#include "server.hpp"

namespace asio = boost::asio;
namespace beast = boost::beast;

using io_context = asio::io_context;
using tcp = asio::ip::tcp;
using tcp_endpt = tcp::endpoint;

using std::cout;
using std::cerr;
using std::endl;
using std::exception;


int main() {
    try {
        io_context ioc{1};
        tcp_endpt endpoint{tcp::v4(), 8080};
        server srv{ioc, endpoint};
        srv.run();
        ioc.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}