#ifndef UI_SERVER_LOGIN_HPP
#define UI_SERVER_LOGIN_HPP

#include <boost/beast/http.hpp>
#include <string>

namespace beast = boost::beast;
namespace http  = beast::http;

class login_handler {
public:
    using request_t  = http::request<http::string_body>;
    using response_t = http::response<http::string_body>;

    static response_t handle_get (const request_t& req);
    static response_t handle_post(const request_t& req);
    static response_t make_app_page(const request_t& req);

private:
    static const std::string login_html_;
    static const std::string app_html_;
};

#endif
