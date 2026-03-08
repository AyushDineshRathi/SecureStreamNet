#include <iostream>
#include <string>

#include "../network/socket_utils.h"
#include "../protocol/packet.h"
#include "../security/aes_encrypt.h"
#include "../security/sha256.h"

// Serializer functions are implemented in protocol/serializer.cpp.
Packet deserialize_packet(const std::string& json_string);

int main() {
    constexpr int kPort = 9000;
    const std::string kAesKey = "0123456789abcdef0123456789abcdef";
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

    const std::string packet_json = receive_data(client_socket);
    if (packet_json.empty()) {
        std::cerr << "[SERVER] No data received from client." << std::endl;
        close_socket(client_socket);
        close_socket(server_socket);
        return 1;
    }
    std::cout << "[SERVER] Packet JSON received" << std::endl;

    try {
        const Packet packet = deserialize_packet(packet_json);
        std::cout << "[SERVER] Packet deserialized" << std::endl;

        if (!packet.validate()) {
            std::cerr << "[SERVER] Packet validation failed" << std::endl;
            close_socket(client_socket);
            close_socket(server_socket);
            return 1;
        }
        std::cout << "[SERVER] Packet validated" << std::endl;

        const std::string expected_checksum = compute_sha256(packet.encrypted_payload);
        std::cout << "[SERVER] Checksum computed" << std::endl;

        if (expected_checksum != packet.checksum) {
            std::cout << "[SERVER] Integrity check FAILED" << std::endl;
            close_socket(client_socket);
            close_socket(server_socket);
            return 1;
        }
        std::cout << "[SERVER] Integrity check passed" << std::endl;

        const std::string frame = decrypt_data(packet.encrypted_payload, kAesKey, packet.iv);
        std::cout << "[SERVER] Payload decrypted" << std::endl;
        std::cout << "[SERVER] Frame received: " << frame << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "[SERVER] Error: " << ex.what() << std::endl;
        close_socket(client_socket);
        close_socket(server_socket);
        return 1;
    }

    close_socket(client_socket);
    close_socket(server_socket);
    return 0;
}
