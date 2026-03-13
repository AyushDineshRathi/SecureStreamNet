#include <iostream>
#include <string>
#include <stdexcept>

#include "../network/socket_utils.h"
#include "../protocol/packet.h"
#include "../security/aes_encrypt.h"
#include "../security/sha256.h"
using namespace std;

// Serializer functions are implemented in protocol/serializer.cpp.
Packet deserialize_packet(const string& json_string);

namespace {
unsigned int hex_nibble(char c) {
    if (c >= '0' && c <= '9') {
        return static_cast<unsigned int>(c - '0');
    }
    if (c >= 'a' && c <= 'f') {
        return static_cast<unsigned int>(10 + (c - 'a'));
    }
    if (c >= 'A' && c <= 'F') {
        return static_cast<unsigned int>(10 + (c - 'A'));
    }
    throw runtime_error("Invalid hex character in IV");
}

string hex_to_bytes(const string& input) {
    if ((input.size() % 2) != 0) {
        throw runtime_error("Invalid hex length for IV");
    }

    string out;
    out.reserve(input.size() / 2);
    for (size_t i = 0; i < input.size(); i += 2) {
        const unsigned int hi = hex_nibble(input[i]);
        const unsigned int lo = hex_nibble(input[i + 1]);
        out.push_back(static_cast<char>((hi << 4) | lo));
    }
    return out;
}
}  // namespace

int main() {
    constexpr int kPort = 9000;
    const string kAesKey = "0123456789abcdef0123456789abcdef";
#ifdef _WIN32
    constexpr socket_handle_t kInvalidSocket = INVALID_SOCKET;
#else
    constexpr socket_handle_t kInvalidSocket = -1;
#endif

    const socket_handle_t server_socket = create_server_socket(kPort);
    if (server_socket == kInvalidSocket) {
        cerr << "[SERVER] Failed to create server socket." << endl;
        return 1;
    }

    cout << "[SERVER] Listening on port " << kPort << "..." << endl;

    const socket_handle_t client_socket = accept_client(server_socket);
    if (client_socket == kInvalidSocket) {
        cerr << "[SERVER] Failed to accept client connection." << endl;
        close_socket(server_socket);
        return 1;
    }
    cout << "[SERVER] Client connected" << endl;

    const string packet_json = receive_data(client_socket);
    if (packet_json.empty()) {
        cerr << "[SERVER] No data received from client." << endl;
        close_socket(client_socket);
        close_socket(server_socket);
        return 1;
    }
    cout << "[SERVER] Packet JSON received" << endl;

    try {
        const Packet packet = deserialize_packet(packet_json);
        cout << "[SERVER] Packet deserialized" << endl;

        if (!packet.validate()) {
            cerr << "[SERVER] Packet validation failed" << endl;
            close_socket(client_socket);
            close_socket(server_socket);
            return 1;
        }
        cout << "[SERVER] Packet validated" << endl;

        const string expected_checksum = compute_sha256(packet.encrypted_payload);
        cout << "[SERVER] Checksum computed" << endl;

        if (expected_checksum != packet.checksum) {
            cout << "[SERVER] Integrity check FAILED" << endl;
            close_socket(client_socket);
            close_socket(server_socket);
            return 1;
        }
        cout << "[SERVER] Integrity check passed" << endl;

        const string iv_raw = hex_to_bytes(packet.iv);
        const string frame = decrypt_data(packet.encrypted_payload, kAesKey, iv_raw);
        cout << "[SERVER] Payload decrypted" << endl;
        cout << "[SERVER] Frame received: " << frame << endl;
    } catch (const exception& ex) {
        cerr << "[SERVER] Error: " << ex.what() << endl;
        close_socket(client_socket);
        close_socket(server_socket);
        return 1;
    }

    close_socket(client_socket);
    close_socket(server_socket);
    return 0;
}
