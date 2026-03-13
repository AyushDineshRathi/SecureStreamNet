#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

#include "../network/socket_utils.h"
#include "../protocol/packet.h"
#include "../security/aes_encrypt.h"
#include "../security/sha256.h"

// Serializer functions are implemented in protocol/serializer.cpp.
string serialize_packet(const Packet& packet);

namespace {
string bytes_to_hex(const string& input) {
    ostringstream oss;
    oss << hex << setfill('0');
    for (unsigned char ch : input) {
        oss << setw(2) << static_cast<int>(ch);
    }
    return oss.str();
}
}

int main() {
    constexpr int kPort = 9000;
    const string kHost = "127.0.0.1";
    const string kAesKey = "0123456789abcdef0123456789abcdef";
#ifdef _WIN32
    constexpr socket_handle_t kInvalidSocket = INVALID_SOCKET;
#else
    constexpr socket_handle_t kInvalidSocket = -1;
#endif

    const socket_handle_t client_socket = create_client_socket();
    if (client_socket == kInvalidSocket) {
        cerr << "[CLIENT] Failed to create client socket." << endl;
        return 1;
    }

    if (!connect_to_server(client_socket, kHost, kPort)) {
        cerr << "[CLIENT] Failed to connect to server." << endl;
        close_socket(client_socket);
        return 1;
    }
    cout << "[CLIENT] Connected to server" << endl;

    try {
        const string frame = "FRAME_1_DATA";
        cout << "[CLIENT] Frame generated" << endl;

        const string iv_raw = generate_random_iv();
        const string encrypted_payload = encrypt_data(frame, kAesKey, iv_raw);
        const string iv_hex = bytes_to_hex(iv_raw);
        cout << "[CLIENT] Payload encrypted" << endl;

        const string checksum = compute_sha256(encrypted_payload);
        cout << "[CLIENT] Checksum computed" << endl;

        const Packet packet(
            "device_01",
            "demo_token",
            static_cast<uint64_t>(time(nullptr)),
            1U,
            iv_hex,
            encrypted_payload,
            checksum);

        const string packet_json = serialize_packet(packet);
        cout << "[CLIENT] Packet serialized" << endl;

        if (!send_data(client_socket, packet_json)) {
            cerr << "[CLIENT] Failed to send packet." << endl;
            close_socket(client_socket);
            return 1;
        }
        cout << "[CLIENT] Packet sent" << endl;
    } catch (const exception& ex) {
        cerr << "[CLIENT] Error: " << ex.what() << endl;
        close_socket(client_socket);
        return 1;
    }

    close_socket(client_socket);
    return 0;
}
