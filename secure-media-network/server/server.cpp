#include <iostream>
#include <string>

#include "../network/socket_utils.h"

#include <unistd.h>

int main() {
    constexpr int kPort = 9000;

    const int server_socket = create_server_socket(kPort);
    if (server_socket < 0) {
        std::cerr << "[SERVER] Failed to create server socket." << std::endl;
        return 1;
    }

    std::cout << "[SERVER] Listening on port " << kPort << "..." << std::endl;

    const int client_socket = accept_client(server_socket);
    if (client_socket < 0) {
        std::cerr << "[SERVER] Failed to accept client connection." << std::endl;
        ::close(server_socket);
        return 1;
    }
    std::cout << "[SERVER] Client connected" << std::endl;

    while (true) {
        const std::string message = receive_data(client_socket);
        if (message.empty()) {
            std::cout << "[SERVER] Client disconnected or receive error." << std::endl;
            break;
        }
        std::cout << "[SERVER] Received: " << message << std::endl;
    }

    ::close(client_socket);
    ::close(server_socket);
    return 0;
}
