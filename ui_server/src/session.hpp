#ifndef UI_SERVER_SESSION_HPP
#define UI_SERVER_SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http  = beast::http;

using tcp              = asio::ip::tcp;
using tcp_socket       = tcp::socket;
using websocket_stream = beast::websocket::stream<tcp_socket>;

class session : public std::enable_shared_from_this<session> {
public:
    explicit session(tcp_socket socket);
    void run();
    void send_message(const std::string& message);

private:
    tcp_socket                        socket_;
    beast::flat_buffer                buffer_;
    http::request<http::string_body>  request_;
    std::unique_ptr<websocket_stream> ws_;

    void do_read_http();
    void handle_http_request();
    void upgrade_to_websocket();
    void send_http_response(http::response<http::string_body> response);
    void do_read_ws();
};

#endif