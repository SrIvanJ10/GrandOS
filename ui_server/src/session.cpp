#include "session.hpp"
#include "login.hpp"

#include <boost/asio.hpp>
#include <iostream>

using error_code = boost::system::error_code;

session::session(tcp_socket socket) : socket_(std::move(socket)) {}

void session::run() {
    do_read_http();
}

void session::do_read_http() {
    http::async_read(
        socket_, buffer_, request_,
        [self = shared_from_this()](error_code ec, std::size_t) {
            if (ec) {
                if (ec != http::error::end_of_stream)
                    std::cerr << "HTTP read error: " << ec.message() << std::endl;
                return;
            }
            self->handle_http_request();
        }
    );
}

void session::handle_http_request() {
    if (beast::websocket::is_upgrade(request_)) {
        upgrade_to_websocket();
        return;
    }

    auto target = std::string(request_.target());

    if (request_.method() == http::verb::get) {
        if (target == "/" || target == "/login") {
            send_http_response(login_handler::handle_get(request_));
            return;
        }
        if (target == "/app") {
            send_http_response(login_handler::make_app_page(request_));
            return;
        }
    }

    if (request_.method() == http::verb::post && target == "/login") {
        send_http_response(login_handler::handle_post(request_));
        return;
    }

    // 404
    http::response<http::string_body> res{http::status::not_found, request_.version()};
    res.set(http::field::content_type, "text/plain");
    res.body() = "Not Found";
    res.prepare_payload();
    send_http_response(std::move(res));
}

void session::upgrade_to_websocket() {
    ws_ = std::make_unique<websocket_stream>(std::move(socket_));
    ws_->async_accept(
        request_,
        [self = shared_from_this()](error_code ec) {
            if (ec) {
                std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
                return;
            }
            self->do_read_ws();
        }
    );
}

void session::send_http_response(http::response<http::string_body> response) {
    response.set(http::field::connection, "close");
    auto res = std::make_shared<http::response<http::string_body>>(std::move(response));
    http::async_write(
        socket_, *res,
        [self = shared_from_this(), res](error_code ec, std::size_t) {
            if (ec)
                std::cerr << "HTTP write error: " << ec.message() << std::endl;
            boost::system::error_code shutdown_ec;
            self->socket_.shutdown(tcp_socket::shutdown_send, shutdown_ec);
        }
    );
}

void session::send_message(const std::string& message) {
    if (!ws_) return;
    ws_->async_write(
        asio::buffer(message),
        [self = shared_from_this()](error_code ec, std::size_t) {
            if (ec)
                std::cerr << "WS write error: " << ec.message() << std::endl;
        }
    );
}

void session::do_read_ws() {
    ws_->async_read(
        buffer_,
        [self = shared_from_this()](error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "WS read error: " << ec.message() << std::endl;
                return;
            }

            auto data = self->buffer_.data();
            std::string message(
                boost::asio::buffers_begin(data),
                boost::asio::buffers_begin(data) + bytes_transferred
            );

            std::cerr << "Recibido: " << message << std::endl;
            self->buffer_.clear();
            self->do_read_ws();
        }
    );
}