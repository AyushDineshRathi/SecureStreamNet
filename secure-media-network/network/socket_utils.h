#ifndef NETWORK_SOCKET_UTILS_H
#define NETWORK_SOCKET_UTILS_H

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
using socket_handle_t = SOCKET;
#else
using socket_handle_t = int;
#endif

// Creates a TCP server socket, binds to INADDR_ANY, and starts listening.
socket_handle_t create_server_socket(int port);

// Accepts one incoming TCP client connection.
socket_handle_t accept_client(socket_handle_t server_socket);

// Creates a TCP client socket.
socket_handle_t create_client_socket();

// Connects an existing TCP client socket to ip:port.
bool connect_to_server(socket_handle_t socket_fd, const std::string& ip, int port);

// Sends all bytes in the provided data string.
bool send_data(socket_handle_t socket_fd, const std::string& data);

// Receives one chunk of data. Returns empty string on disconnect/error.
std::string receive_data(socket_handle_t socket_fd);

// Closes a socket handle.
void close_socket(socket_handle_t socket_fd);

#endif  // NETWORK_SOCKET_UTILS_H
