#include "login.hpp"

const std::string login_handler::login_html_ = R"html(<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GrandOS - Login</title>
</head>
<body>
  <h1>GrandOS</h1>
  <form id="loginForm">
    <div>
      <label for="username">Usuario:</label>
      <input type="text" id="username" name="username" autocomplete="username" required />
    </div>
    <div>
      <label for="password">Contrase&ntilde;a:</label>
      <input type="password" id="password" name="password" autocomplete="current-password" required />
    </div>
    <button type="submit">Entrar</button>
  </form>
  <script>
    document.getElementById('loginForm').addEventListener('submit', function(e) {
      e.preventDefault();
      // TODO: enviar credenciales al servidor para validaci&oacute;n
      window.location.href = '/app';
    });
  </script>
</body>
</html>)html";

const std::string login_handler::app_html_ = R"html(<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GrandOS</title>
</head>
<body>
  <h1>GrandOS</h1>
  <p id="status">Conectando...</p>
  <button onclick="sendMessage()">Enviar mensaje</button>
  <div id="log" style="border:1px solid #ccc; padding:10px; height:300px; overflow-y:auto;"></div>
  <script>
    function log(msg) {
      const el = document.getElementById('log');
      el.innerHTML += msg + '<br>';
      el.scrollTop = el.scrollHeight;
    }

    const ws = new WebSocket('ws://' + window.location.host);
    ws.onopen = function() {
      document.getElementById('status').textContent = 'Conectado';
      log('Conectado al servidor');
    };
    ws.onmessage = function(event) {
      log('Recibido: ' + event.data);
    };
    ws.onclose = function() {
      document.getElementById('status').textContent = 'Desconectado';
      log('Desconectado del servidor');
    };
    ws.onerror = function(error) {
      log('Error WebSocket');
      console.error('Error WebSocket:', error);
    };

    function sendMessage() {
      if (ws && ws.readyState === WebSocket.OPEN) {
        ws.send('Hola desde navegador!');
        log('Enviado: Hola desde navegador!');
      }
    }
  </script>
</body>
</html>)html";

login_handler::response_t login_handler::handle_get(const request_t& req) {
    response_t res{http::status::ok, req.version()};
    res.set(http::field::content_type, "text/html; charset=utf-8");
    res.body() = login_html_;
    res.prepare_payload();
    return res;
}

login_handler::response_t login_handler::handle_post(const request_t& req) {
    // TODO: validar credenciales
    response_t res{http::status::see_other, req.version()};
    res.set(http::field::location, "/app");
    res.prepare_payload();
    return res;
}

login_handler::response_t login_handler::make_app_page(const request_t& req) {
    response_t res{http::status::ok, req.version()};
    res.set(http::field::content_type, "text/html; charset=utf-8");
    res.body() = app_html_;
    res.prepare_payload();
    return res;
}
