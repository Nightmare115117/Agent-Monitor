#include <crow.h>
#include <crow/middlewares/cors.h>
#include "stdexcept"
#include "unistd.h"
#include "iostream"

using namespace std;
using App = crow::App<crow::CORSHandler>;

int main() {

    App app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
    .headers("Content-Type", "Authorization")
    .methods("GET"_method)
    .origin("*");

    CROW_WEBSOCKET_ROUTE(app, "/ws")
    .onopen([](crow::websocket::connection& conn){
        try {
        char host [HOST_NAME_MAX];
        size_t longitud = HOST_NAME_MAX;
        int r = gethostname(host, longitud);
        if (r != 0) 
            throw runtime_error("No se pudo obtener el hostname");
        string hostname = host;
        conn.send_text("El host: " + hostname + " ha respondido correctamente");
        } catch (const runtime_error& e) {
            string error = strerror(errno);
            cerr << error << endl;
            conn.close();
        }
    })
    .onmessage([](crow::websocket::connection& conn, const string& message, bool is_binary) {
        
    })
    .onclose([](crow::websocket::connection& conn, const string& message, uint16_t code) {
    });


    app.port(8080).multithreaded().run();
}