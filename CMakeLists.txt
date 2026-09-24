cmake_minimum_required(VERSION 3.15)
project(agent_monitor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Compilar las pruebas unitarias" OFF)

include(FetchContent)

FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.3.3
)

FetchContent_MakeAvailable(Crow)

find_package(Threads REQUIRED)

# Toda la lógica de lectura (CPU, Disk, Network, RAM) va en una librería.
# main.cpp se queda fuera para que las pruebas no arrastren otro main.
file(GLOB_RECURSE SYSTEM_SOURCES CONFIGURE_DEPENDS "src/System/*.cpp")

add_library(system_lib STATIC ${SYSTEM_SOURCES})
target_include_directories(system_lib PUBLIC src)
# Si los módulos de System no usan Crow, mueve Crow::Crow al ejecutable.
target_link_libraries(system_lib PUBLIC
    Crow::Crow
    Threads::Threads)

add_executable(agent_monitor src/main.cpp)
target_link_libraries(agent_monitor PRIVATE system_lib)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()