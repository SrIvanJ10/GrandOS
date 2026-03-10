#ifndef UI_SERVER_SESSION_HPP
#define UI_SERVER_SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace asio = boost::asio;
namespace beast = boost::beast;

using tcp = asio::ip::tcp;
using tcp_socket = asio::ip::tcp::socket;
using websocket_stream = beast::websocket::stream<tcp_socket>;

class session : public std::enable_shared_from_this<session> {
    public:
        explicit session(tcp_socket socket);
        void run();
        void send_message(const std::string& message);

    private:
        websocket_stream ws_;
        beast::flat_buffer buffer_;

        void do_read();
};

#endif