# Agent Monitor

**English version:** [English](#english)

Monitor de sistema desarrollado en C++20. La aplicación se ejecuta directamente en Linux y levanta un servidor Crow con un endpoint WebSocket en `/ws`, en el puerto `8080`. Al conectar, responde con el hostname del equipo.

## Restricción de ejecución

Este proyecto debe ejecutarse como un proceso nativo en la máquina Linux. **No se debe usar Docker, Kubernetes ni tecnologías o servicios similares**: no usar contenedores, Docker Compose, Podman, runtimes de contenedores, Helm, Minikube, servicios cloud ni plataformas de orquestación o despliegue administrado. La compilación y ejecución se realizan localmente con las herramientas del sistema.

## Estado

- RAM: lector de memoria y swap mediante `/proc/meminfo`.
- Red: lector de contadores de interfaces mediante `/proc/net/dev`, limitado a interfaces `enp`, `flannel` y `cni`.
- CPU y disco: módulos presentes, pero sus lectores todavía no están implementados.
- WebSocket: `/ws` confirma la conexión devolviendo el hostname. La publicación de métricas por WebSocket aún no está conectada.

## Requisitos

- Linux con `/proc` montado.
- Compilador de C++ compatible con C++20.
- CMake 3.15 o posterior.
- Git y acceso a Internet durante la configuración inicial, para que CMake descargue Crow. Para compilar las pruebas también se descarga GoogleTest.

## Compilar y ejecutar

Desde la raíz del repositorio:

```sh
cmake -S . -B build
cmake --build build
./build/agent_monitor
```

El servidor escucha en el puerto `8080`. Para comprobarlo, conecta un cliente WebSocket a `ws://localhost:8080/ws`.

## Pruebas

Las pruebas se habilitan con `BUILD_TESTS`:

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Estructura

- `src/main.cpp`: inicio del servidor Crow y configuración del WebSocket.
- `src/System/`: modelos y lectores de recursos del sistema.
- `tests/`: pruebas unitarias.
- `CMakeLists.txt`: configuración de compilación y dependencias.

# English

Agent Monitor is a system monitoring project written in C++20. It runs directly on Linux and starts a Crow server with a WebSocket endpoint at `/ws` on port `8080`. When a client connects, the server responds with the machine's hostname.

## Execution Constraint

This project must run as a native process on a Linux machine. **Do not use Docker, Kubernetes, or similar technologies or services**: this includes containers, Docker Compose, Podman, container runtimes, Helm, Minikube, cloud services, and managed orchestration or deployment platforms. Build and run the application locally using the system's development tools.

## Status

- RAM: memory and swap reader using `/proc/meminfo`.
- Network: interface counters read from `/proc/net/dev`, limited to `enp`, `flannel`, and `cni` interfaces.
- CPU and disk: modules exist, but their readers are not implemented yet.
- WebSocket: `/ws` confirms the connection by returning the hostname. Streaming metrics over WebSocket is not connected yet.

## Requirements

- Linux with `/proc` mounted.
- A C++ compiler that supports C++20.
- CMake 3.15 or later.
- Git and Internet access during the initial configuration so CMake can download Crow. GoogleTest is also downloaded when building the tests.

## Build and Run

From the repository root:

```sh
cmake -S . -B build
cmake --build build
./build/agent_monitor
```

The server listens on port `8080`. Connect a WebSocket client to `ws://localhost:8080/ws` to verify it.

## Tests

Enable tests with `BUILD_TESTS`:

```sh
cmake -S . -B build -DBUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Project Structure

- `src/main.cpp`: starts the Crow server and configures the WebSocket endpoint.
- `src/System/`: system resource models and readers.
- `tests/`: unit tests.
- `CMakeLists.txt`: build and dependency configuration.
