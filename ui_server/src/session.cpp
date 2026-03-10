#include "session.hpp"

#include <boost/asio.hpp>
#include <iostream>

using error_code = boost::system::error_code;

session::session(tcp_socket socket) : ws_(std::move(socket)) {}

void session::run(){
    // Accept the websocket handshake

    ws_.async_accept(
        [self = shared_from_this()](error_code ec){
            if (ec){ // Handle error
                return;
            }
            // Start reading messages or handling the connection
            self->do_read();
        }
    );
}

void session::send_message(const std::string& message){

    ws_.async_write(
        asio::buffer(message),
        [self = shared_from_this()](error_code ec, std::size_t bytes_transferred){
            if (ec){ // Handle error
                return;
            }
            // Message sent successfully, you can handle post-send logic here
        }
    );
}

void session::do_read(){
    ws_.async_read(
        buffer_,
        [self = shared_from_this()](error_code ec, std::size_t bytes_transferred){
            if (ec){
                std::cerr << "Error en lectura: " << ec.message() << std::endl;
                return;
            }
            
            std::cerr << "Bytes recibidos: " << bytes_transferred << std::endl;
            
            // Convertir buffer a string
            auto data = self->buffer_.data();
            std::string message(
                boost::asio::buffers_begin(data),
                boost::asio::buffers_begin(data) + bytes_transferred
            );
            
            // Imprimir en consolaVM135:1 Connecting to 'ws://localhost:8080/' violates the following Content Security Policy directive: "connect-src chrome://resources chrome://theme 'self'". The action has been blocked.
            std::cerr << "Recibido: " << message << std::endl;
            
            // Limpiar buffer para siguiente iteración
            self->buffer_.clear();
            
            // Leer siguiente mensaje
            self->do_read();
        }
    );
}