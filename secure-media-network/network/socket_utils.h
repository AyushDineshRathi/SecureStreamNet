#ifndef NETWORK_SOCKET_UTILS_H
#define NETWORK_SOCKET_UTILS_H

#include <string>

// Creates a TCP server socket, binds to INADDR_ANY, and starts listening.
int create_server_socket(int port);

// Accepts one incoming TCP client connection.
int accept_client(int server_socket);

// Creates a TCP client socket.
int create_client_socket();

// Connects an existing TCP client socket to ip:port.
bool connect_to_server(int socket_fd, const std::string& ip, int port);

// Sends all bytes in the provided data string.
bool send_data(int socket_fd, const std::string& data);

// Receives one chunk of data. Returns empty string on disconnect/error.
std::string receive_data(int socket_fd);

#endif  // NETWORK_SOCKET_UTILS_H
