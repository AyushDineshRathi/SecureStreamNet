#include "socket_utils.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace {
constexpr int kDefaultBacklog = 5;
}

int create_server_socket(int port) {
    const int server_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "[NETWORK] socket() failed: " << std::strerror(errno) << std::endl;
        return -1;
    }

    const int reuse_addr = 1;
    if (::setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0) {
        std::cerr << "[NETWORK] setsockopt(SO_REUSEADDR) failed: " << std::strerror(errno) << std::endl;
        ::close(server_fd);
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(static_cast<uint16_t>(port));

    if (::bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "[NETWORK] bind() failed: " << std::strerror(errno) << std::endl;
        ::close(server_fd);
        return -1;
    }

    if (::listen(server_fd, kDefaultBacklog) < 0) {
        std::cerr << "[NETWORK] listen() failed: " << std::strerror(errno) << std::endl;
        ::close(server_fd);
        return -1;
    }

    return server_fd;
}

int accept_client(int server_socket) {
    const int client_fd = ::accept(server_socket, nullptr, nullptr);
    if (client_fd < 0) {
        std::cerr << "[NETWORK] accept() failed: " << std::strerror(errno) << std::endl;
        return -1;
    }
    return client_fd;
}

int create_client_socket() {
    const int client_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        std::cerr << "[NETWORK] socket() failed: " << std::strerror(errno) << std::endl;
        return -1;
    }
    return client_fd;
}

bool connect_to_server(int socket_fd, const std::string& ip, int port) {
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(static_cast<uint16_t>(port));

    if (::inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "[NETWORK] inet_pton() failed for '" << ip << "'" << std::endl;
        return false;
    }

    if (::connect(socket_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "[NETWORK] connect() failed: " << std::strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool send_data(int socket_fd, const std::string& data) {
    size_t total_sent = 0;
    while (total_sent < data.size()) {
        const auto bytes_sent = ::send(
            socket_fd,
            data.data() + total_sent,
            data.size() - total_sent,
            0);
        if (bytes_sent < 0) {
            std::cerr << "[NETWORK] send() failed: " << std::strerror(errno) << std::endl;
            return false;
        }
        total_sent += static_cast<size_t>(bytes_sent);
    }
    return true;
}

std::string receive_data(int socket_fd) {
    char buffer[4096];
    const auto bytes_received = ::recv(socket_fd, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        std::cerr << "[NETWORK] recv() failed: " << std::strerror(errno) << std::endl;
        return "";
    }
    if (bytes_received == 0) {
        return "";
    }
    return std::string(buffer, static_cast<size_t>(bytes_received));
}
