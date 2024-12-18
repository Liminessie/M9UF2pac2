#include <winsock2.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#define PUERTO 12345

void receive_messages(SOCKET client) {
    char buffer[1024];
    while (true) {
        int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << buffer << "\n";
        }
    }
}

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    sockaddr_in server_address;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PUERTO);

    connect(client_socket, (sockaddr*)&server_address, sizeof(server_address));
    std::thread(receive_messages, client_socket).detach();

    char message[1024];
    while (true) {
        std::cin.getline(message, sizeof(message));
        send(client_socket, message, strlen(message), 0);
    }

    closesocket(client_socket);
    WSACleanup();
    return 0;
}
