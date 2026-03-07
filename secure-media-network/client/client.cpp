#include <iostream>
#include <string>

#include "../network/socket_utils.h"

#include <unistd.h>

int main() {
    constexpr int kPort = 9000;
    const std::string kHost = "127.0.0.1";
    const std::string kTestMessage = "HELLO_SERVER";

    const int client_socket = create_client_socket();
    if (client_socket < 0) {
        std::cerr << "[CLIENT] Failed to create client socket." << std::endl;
        return 1;
    }

    if (!connect_to_server(client_socket, kHost, kPort)) {
        std::cerr << "[CLIENT] Failed to connect to server." << std::endl;
        ::close(client_socket);
        return 1;
    }
    std::cout << "[CLIENT] Connected to server" << std::endl;

    if (!send_data(client_socket, kTestMessage)) {
        std::cerr << "[CLIENT] Failed to send message." << std::endl;
        ::close(client_socket);
        return 1;
    }
    std::cout << "[CLIENT] Message sent" << std::endl;

    ::close(client_socket);
    return 0;
}
