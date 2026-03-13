#include "socket_utils.h"

#include <cstdint>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace {
constexpr int kReceiveBufferSize = 4096;

#ifdef _WIN32
bool ensure_winsock_initialized() {
    static bool initialized = false;
    if (initialized) {
        return true;
    }

    WSADATA wsa_data{};
    const int result = ::WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        std::cerr << "[NETWORK] WSAStartup() failed with code=" << result << std::endl;
        return false;
    }

    initialized = true;
    return true;
}

int last_socket_error() {
    return ::WSAGetLastError();
}
#else
int last_socket_error() {
    return errno;
}
#endif

bool is_invalid_socket(socket_handle_t socket_fd) {
#ifdef _WIN32
    return socket_fd == INVALID_SOCKET;
#else
    return socket_fd < 0;
#endif
}

socket_handle_t invalid_socket_value() {
#ifdef _WIN32
    return INVALID_SOCKET;
#else
    return -1;
#endif
}

int to_send_length(size_t value) {
    return static_cast<int>(value);
}

}  // namespace

socket_handle_t create_server_socket(int port) {
    return create_server_socket(port, 5);
}

socket_handle_t create_server_socket(int port, int backlog) {
#ifdef _WIN32
    if (!ensure_winsock_initialized()) {
        return invalid_socket_value();
    }
#endif

    const socket_handle_t server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (is_invalid_socket(server_fd)) {
        std::cerr << "[NETWORK] socket() failed. code=" << last_socket_error() << std::endl;
        return invalid_socket_value();
    }

    const int reuse_addr = 1;
    if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse_addr), sizeof(reuse_addr)) < 0) {
        std::cerr << "[NETWORK] setsockopt(SO_REUSEADDR) failed. code=" << last_socket_error() << std::endl;
        close_socket(server_fd);
        return invalid_socket_value();
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(static_cast<uint16_t>(port));

    if (::bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "[NETWORK] bind() failed. code=" << last_socket_error() << std::endl;
        close_socket(server_fd);
        return invalid_socket_value();
    }

    if (::listen(server_fd, backlog) < 0) {
        std::cerr << "[NETWORK] listen() failed. code=" << last_socket_error() << std::endl;
        close_socket(server_fd);
        return invalid_socket_value();
    }

    return server_fd;
}

socket_handle_t accept_client(socket_handle_t server_socket) {
    const socket_handle_t client_fd = ::accept(server_socket, nullptr, nullptr);
    if (is_invalid_socket(client_fd)) {
        std::cerr << "[NETWORK] accept() failed. code=" << last_socket_error() << std::endl;
        return invalid_socket_value();
    }
    return client_fd;
}

socket_handle_t create_client_socket() {
#ifdef _WIN32
    if (!ensure_winsock_initialized()) {
        return invalid_socket_value();
    }
#endif

    const socket_handle_t client_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (is_invalid_socket(client_fd)) {
        std::cerr << "[NETWORK] socket() failed. code=" << last_socket_error() << std::endl;
        return invalid_socket_value();
    }
    return client_fd;
}

bool connect_to_server(socket_handle_t socket_fd, const std::string& ip, int port) {
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(static_cast<uint16_t>(port));

#ifdef _WIN32
    const unsigned long ip_addr = ::inet_addr(ip.c_str());
    if (ip_addr == INADDR_NONE) {
        std::cerr << "[NETWORK] inet_addr() failed for '" << ip << "'" << std::endl;
        return false;
    }
    server_addr.sin_addr.s_addr = ip_addr;
#else
    if (::inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "[NETWORK] inet_pton() failed for '" << ip << "'" << std::endl;
        return false;
    }
#endif

    if (::connect(socket_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "[NETWORK] connect() failed. code=" << last_socket_error() << std::endl;
        return false;
    }

    return true;
}

bool send_data(socket_handle_t socket_fd, const std::string& data) {
    size_t total_sent = 0;
    while (total_sent < data.size()) {
        const auto bytes_sent = ::send(
            socket_fd,
            data.data() + total_sent,
            to_send_length(data.size() - total_sent),
            0);
        if (bytes_sent < 0) {
            std::cerr << "[NETWORK] send() failed. code=" << last_socket_error() << std::endl;
            return false;
        }
        total_sent += static_cast<size_t>(bytes_sent);
    }
    return true;
}

std::string receive_data(socket_handle_t socket_fd) {
    std::string data;
    char buffer[kReceiveBufferSize];

    while (true) {
        const auto bytes_received = ::recv(socket_fd, buffer, static_cast<int>(sizeof(buffer)), 0);
        if (bytes_received < 0) {
            std::cerr << "[NETWORK] recv() failed. code=" << last_socket_error() << std::endl;
            return "";
        }
        if (bytes_received == 0) {
            break;
        }

        data.append(buffer, static_cast<size_t>(bytes_received));

        if (bytes_received < static_cast<int>(sizeof(buffer))) {
            break;
        }
    }

    return data;
}

void close_socket(socket_handle_t socket_fd) {
#ifdef _WIN32
    ::closesocket(socket_fd);
#else
    ::close(socket_fd);
#endif
}
