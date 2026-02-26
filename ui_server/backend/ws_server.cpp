#include <libwebsockets.h>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

static const std::string RESPONSE = "Bien, me sirve";

static int callback_ws(lws* wsi, lws_callback_reasons reason, void* /*user*/, void* in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            std::cout << "Cliente conectado\n";
            lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_SERVER_WRITEABLE: {
            // libwebsockets requiere LWS_PRE bytes de padding antes del payload
            std::vector<unsigned char> buf(LWS_PRE + RESPONSE.size());
            std::memcpy(buf.data() + LWS_PRE, RESPONSE.data(), RESPONSE.size());
            lws_write(wsi, buf.data() + LWS_PRE, RESPONSE.size(), LWS_WRITE_TEXT);
            break;
        }

        case LWS_CALLBACK_RECEIVE:
            std::cout << "Mensaje recibido: " << std::string((char*)in, len) << '\n';
            break;

        case LWS_CALLBACK_CLOSED:
            std::cout << "Cliente desconectado\n";
            break;

        default:
            break;
    }
    return 0;
}

static lws_protocols protocols[] = {
    { "ws-protocol", callback_ws, 0, 4096, 0, nullptr, 0 },
    LWS_PROTOCOL_LIST_TERM
};

int main() {
    lws_context_creation_info info{};
    info.port = 9001;
    info.protocols = protocols;
    info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

    lws_set_log_level(LLL_ERR | LLL_WARN, nullptr);

    lws_context* ctx = lws_create_context(&info);
    if (!ctx) {
        std::cerr << "Error al crear contexto libwebsockets\n";
        return 1;
    }

    std::cout << "WebSocket server escuchando en ws://localhost:9001\n";

    while (lws_service(ctx, 0) >= 0) {}

    lws_context_destroy(ctx);
    return 0;
}
