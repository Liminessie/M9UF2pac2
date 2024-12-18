#include <winsock2.h>
#include <iostream>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
#define PUERTO 12345

std::vector<SOCKET> clientes;

void send_to_all(const char* mensaje, SOCKET emisor) {
    for (SOCKET cliente : clientes) {
        if (cliente != emisor) {
            send(cliente, mensaje, strlen(mensaje), 0);
        }
    }
}

void handle_client(SOCKET cliente_socket) {
    char buffer[1024];
    while (true) {
        int bytes_recibidos = recv(cliente_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_recibidos <= 0) {
            // Eliminar cliente desconectado
            for (auto it = clientes.begin(); it != clientes.end(); ++it) {
                if (*it == cliente_socket) {
                    clientes.erase(it);
                    break;
                }
            }
            closesocket(cliente_socket);
            return;
        }
        buffer[bytes_recibidos] = '\0';
        send_to_all(buffer, cliente_socket);
    }
}

int main() {
    WSADATA wsaData;
    SOCKET server_socket;
    sockaddr_in server_address;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PUERTO);

    bind(server_socket, (sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    std::cout << "Socket configurado correctamente!! hehe"; // Asi bien?
    while (true) {
        SOCKET cliente_socket = accept(server_socket, nullptr, nullptr);
        clientes.push_back(cliente_socket);
        std::thread(handle_client, cliente_socket).detach();
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;

    //Compilar el server asi:  g++ server.cpp -o server -lws2_32
    //Compilar el cliente asi: g++ client.cpp -o client -lws2_32
}

