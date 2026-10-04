# Agent Monitor

**English version:** [English](#english)

Monitor del sistema desarrollado en C++20 para Linux. El proyecto compila con CMake y levanta un servidor Crow con un endpoint WebSocket en `/ws` en el puerto `8080`. Al abrir la conexión, el servidor responde con el hostname del equipo y, cuando el cliente envía el mensaje `metrics`, devuelve un JSON con el estado actual de CPU, RAM, disco y red.

## Restricción de ejecución

Este repositorio debe ejecutarse como un proceso nativo en la máquina Linux. **No se debe usar Docker, Kubernetes ni tecnologías o servicios similares**: no usar contenedores, Docker Compose, Podman, runtimes de contenedores, Helm, Minikube, servicios cloud ni plataformas de orquestación o despliegue administrado. La compilación y ejecución se realizan localmente con herramientas del sistema.

## Estado actual

- CPU: lector de `/proc/stat` para calcular el uso del procesador a partir de muestras consecutivas.
- RAM: lectura de `/proc/meminfo` con total, disponible, cache y uso de swap.
- Red: lectura de `/proc/net/dev` filtrando interfaces `enp`, `flannel` y `cni`.
- Disco: módulo de monitorización de almacenamiento con datos de uso, libre y porcentaje de ocupación.
- WebSocket: `/ws` responde al abrir la conexión y acepta el mensaje `metrics` para devolver las métricas en JSON.

## Requisitos

- Linux con `/proc` montado.
- Compilador de C++ compatible con C++20.
- CMake 3.15 o superior.
- Git y acceso a Internet durante la configuración inicial, para que CMake descargue Crow y nlohmann/json. Cuando se habilitan pruebas también se descarga GoogleTest.

## Compilar y ejecutar

Desde la raíz del repositorio:

```sh
cmake -S . -B build
cmake --build build
./build/agent_monitor
```

El servidor queda escuchando en el puerto `8080`.

## API WebSocket

Puedes comprobarlo conectándote con un cliente WebSocket a `ws://localhost:8080/ws`.

- Al abrir la conexión, el servidor responde con un mensaje del estilo:

```text
El host: <hostname> ha respondido correctamente
```

- Para solicitar métricas, envía el mensaje:

```text
metrics
```

- El servidor responde con un JSON que incluye datos de CPU, RAM, disco y red, por ejemplo:

```json
{
  "CPU": { "usage": 12.5 },
  "ram": { "total": 16384, "available": 9012, "cache": 2400, "swapTotal": 0, "swapUsed": 0 },
  "disck": { "device": "/", "free": 123456, "used": 45678, "usage": 27.3, "usageP": 27.3 },
  "red": {
    "enp0s3": { "recived": 154321, "recivedPackets": 1200, "transmitted": 97000, "trasmittedPackets": 840 }
  }
}
```

## Pruebas

Las pruebas se habilitan con `BUILD_TESTS`:

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Estructura

- `src/main.cpp`: inicio del servidor Crow y configuración del WebSocket.
- `src/System/CPU/`: lector y cálculo de uso de CPU.
- `src/System/RAM/`: monitor de memoria y swap.
- `src/System/Network/`: contadores de red por interfaz.
- `src/System/Disk/`: métricas de almacenamiento.
- `tests/`: pruebas unitarias.
- `CMakeLists.txt`: configuración del proyecto y dependencias.

# English

Agent Monitor is a C++20 Linux system monitoring project. It is built with CMake and runs a Crow server with a WebSocket endpoint at `/ws` on port `8080`. When the connection opens, the server responds with the machine hostname, and when a client sends the message `metrics`, it returns a JSON payload with current CPU, RAM, disk, and network data.

## Execution constraint

This repository must run as a native Linux process. **Do not use Docker, Kubernetes, or similar technologies or services**: this includes containers, Docker Compose, Podman, container runtimes, Helm, Minikube, cloud services, and managed orchestration or deployment platforms. Build and run the project locally with the system toolchain.

## Current status

- CPU: reads `/proc/stat` to calculate processor usage from consecutive samples.
- RAM: reads `/proc/meminfo` and exposes total, available, cache, and swap usage.
- Network: reads `/proc/net/dev` and filters interfaces such as `enp`, `flannel`, and `cni`.
- Disk: storage monitor that reports free space, used space, and usage percentage.
- WebSocket: `/ws` responds on connection and accepts the `metrics` message to return system information in JSON.

## Requirements

- Linux with `/proc` mounted.
- A C++ compiler compatible with C++20.
- CMake 3.15 or newer.
- Git and Internet access during initial setup so CMake can fetch Crow and nlohmann/json. GoogleTest is also downloaded when tests are enabled.

## Build and run

From the repository root:

```sh
cmake -S . -B build
cmake --build build
./build/agent_monitor
```

The server listens on port `8080`.

## WebSocket API

You can verify it by connecting a WebSocket client to `ws://localhost:8080/ws`.

- When the connection opens, the server responds with a message like:

```text
El host: <hostname> ha respondido correctamente
```

- To request metrics, send this message:

```text
metrics
```

- The server replies with a JSON document containing CPU, RAM, disk, and network values, for example:

```json
{
  "CPU": { "usage": 12.5 },
  "ram": { "total": 16384, "available": 9012, "cache": 2400, "swapTotal": 0, "swapUsed": 0 },
  "disck": { "device": "/", "free": 123456, "used": 45678, "usage": 27.3, "usageP": 27.3 },
  "red": {
    "enp0s3": { "recived": 154321, "recivedPackets": 1200, "transmitted": 97000, "trasmittedPackets": 840 }
  }
}
```

## Tests

Enable tests with `BUILD_TESTS`:

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Project structure

- `src/main.cpp`: starts the Crow server and configures the WebSocket endpoint.
- `src/System/CPU/`: reads and calculates CPU usage.
- `src/System/RAM/`: monitors memory and swap.
- `src/System/Network/`: collects per-interface network metrics.
- `src/System/Disk/`: reports storage usage.
- `tests/`: unit tests.
- `CMakeLists.txt`: project configuration and dependencies.
