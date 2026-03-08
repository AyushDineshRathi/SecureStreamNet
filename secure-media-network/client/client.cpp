#include <iostream>
#include <string>
#include <ctime>

#include "../network/socket_utils.h"
#include "../protocol/packet.h"
#include "../security/aes_encrypt.h"
#include "../security/sha256.h"

// Serializer functions are implemented in protocol/serializer.cpp.
std::string serialize_packet(const Packet& packet);

int main() {
    constexpr int kPort = 9000;
    const std::string kHost = "127.0.0.1";
    const std::string kAesKey = "0123456789abcdef0123456789abcdef";
#ifdef _WIN32
    constexpr socket_handle_t kInvalidSocket = INVALID_SOCKET;
#else
    constexpr socket_handle_t kInvalidSocket = -1;
#endif

    const socket_handle_t client_socket = create_client_socket();
    if (client_socket == kInvalidSocket) {
        std::cerr << "[CLIENT] Failed to create client socket." << std::endl;
        return 1;
    }

    if (!connect_to_server(client_socket, kHost, kPort)) {
        std::cerr << "[CLIENT] Failed to connect to server." << std::endl;
        close_socket(client_socket);
        return 1;
    }
    std::cout << "[CLIENT] Connected to server" << std::endl;

    try {
        const std::string frame = "FRAME_1_DATA";
        std::cout << "[CLIENT] Frame generated" << std::endl;

        const std::string iv = generate_random_iv();
        const std::string encrypted_payload = encrypt_data(frame, kAesKey, iv);
        std::cout << "[CLIENT] Payload encrypted" << std::endl;

        const std::string checksum = compute_sha256(encrypted_payload);
        std::cout << "[CLIENT] Checksum computed" << std::endl;

        const Packet packet(
            "device_01",
            "demo_token",
            static_cast<std::uint64_t>(std::time(nullptr)),
            1U,
            iv,
            encrypted_payload,
            checksum);

        const std::string packet_json = serialize_packet(packet);
        std::cout << "[CLIENT] Packet serialized" << std::endl;

        if (!send_data(client_socket, packet_json)) {
            std::cerr << "[CLIENT] Failed to send packet." << std::endl;
            close_socket(client_socket);
            return 1;
        }
        std::cout << "[CLIENT] Packet sent" << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "[CLIENT] Error: " << ex.what() << std::endl;
        close_socket(client_socket);
        return 1;
    }

    close_socket(client_socket);
    return 0;
}
