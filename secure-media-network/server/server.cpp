#include <iostream>
#include <string>

#include "../network/socket_utils.h"

int main() {
    constexpr int kPort = 9000;
#ifdef _WIN32
    constexpr socket_handle_t kInvalidSocket = INVALID_SOCKET;
#else
    constexpr socket_handle_t kInvalidSocket = -1;
#endif

    const socket_handle_t server_socket = create_server_socket(kPort);
    if (server_socket == kInvalidSocket) {
        std::cerr << "[SERVER] Failed to create server socket." << std::endl;
        return 1;
    }

    std::cout << "[SERVER] Listening on port " << kPort << "..." << std::endl;

    const socket_handle_t client_socket = accept_client(server_socket);
    if (client_socket == kInvalidSocket) {
        std::cerr << "[SERVER] Failed to accept client connection." << std::endl;
        close_socket(server_socket);
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

    close_socket(client_socket);
    close_socket(server_socket);
    return 0;
}
