#include <crow.h>
#include <crow/middlewares/cors.h>
#include <nlohmann/json.hpp>
#include "stdexcept"
#include "unistd.h"
#include "iostream"
#include "System/System.hpp"

using namespace std;
using App = crow::App<crow::CORSHandler>;
using json = nlohmann::json;

json parser();

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
        try {
            if (message == "metrics") {
                json dto = parser();
                conn.send_text(dto.dump());
            } else {
                throw logic_error("Mensaje Incorrecto intente de nuevo");
            }
        } catch (const exception& e) {
            conn.send_text(e.what());
        }
    })
    .onclose([](crow::websocket::connection& conn, const string& message, uint16_t code) {
    })
    .onerror([](crow::websocket::connection& conn, const string& message) {
    });

    app.port(8080).multithreaded().run();
}

json parser() {
    json dto;
    json datos;
    const CPU cpu = CPUReader::readCPU();
    if (cpu.getMetrics().usagePercent.has_value() == false)
        throw runtime_error("No contiene datos");
    datos["usage"] = cpu.getMetrics().usagePercent.value();
    dto["CPU"] = datos;
    datos.clear();
    if (!datos.empty())
        throw runtime_error("No se pudo limpiar");
                
    const RAM ram = RAMReader::reader();
    datos["total"] = ram.getTotal();
    datos["available"] = ram.getAvailable();
    datos["cache"] = ram.getCache();
    datos["swapTotal"] = ram.getSwapTotal();
    datos["swapUsed"] = ram.getSwapUsed();
    dto["ram"] = datos;
    datos.clear();
    if (!datos.empty())
        throw runtime_error("No se pudo limpiar");
    const Disk disco = DiskReader::reader();
    datos["device"] = disco.getDevice();
    datos["free"] = disco.getFree();
    datos["used"] = disco.getUsed();
    datos["usage"] = disco.getUsage();
    datos["usageP"] = disco.getUsageP();
    dto["disck"] = datos;
    datos.clear();
    if (!datos.empty())
        throw runtime_error("No se pudo limpiar");
    const vector<Network> redes = NetworkReader::reader();
    for (const auto& red : redes) {
        datos[red.getInterfaceName()]["recived"] = red.getReceivedBytes();
        datos[red.getInterfaceName()]["recivedPackets"] = red.getReceivedPackets();
        datos[red.getInterfaceName()]["transmitted"] = red.getTransmittedBytes();
        datos[red.getInterfaceName()]["trasmittedPackets"] = red.getTransmittedPackets();
        
    }
    dto["red"] = datos;
    datos.clear();
    return dto;      
}