#ifndef PROTOCOL_PACKET_H
#define PROTOCOL_PACKET_H

#include <cstdint>
#include <iostream>
#include <string>

// Represents one secure media packet exchanged between peers.
struct Packet {
    std::string sender_id;
    std::string auth_token;
    std::uint64_t timestamp;
    std::uint32_t sequence_number;
    std::uint32_t payload_size;
    std::string iv;
    std::string encrypted_payload;
    std::string checksum;

    // Default constructor for creating an empty packet.
    Packet()
        : sender_id(""),
          auth_token(""),
          timestamp(0),
          sequence_number(0),
          payload_size(0),
          iv(""),
          encrypted_payload(""),
          checksum("") {}

    // Parameterized constructor for creating a fully populated packet.
    Packet(
        const std::string& sender_id_in,
        const std::string& auth_token_in,
        std::uint64_t timestamp_in,
        std::uint32_t sequence_number_in,
        const std::string& iv_in,
        const std::string& encrypted_payload_in,
        const std::string& checksum_in)
        : sender_id(sender_id_in),
          auth_token(auth_token_in),
          timestamp(timestamp_in),
          sequence_number(sequence_number_in),
          payload_size(static_cast<std::uint32_t>(encrypted_payload_in.size())),
          iv(iv_in),
          encrypted_payload(encrypted_payload_in),
          checksum(checksum_in) {}

    // Basic structural checks before deeper protocol/security validation.
    bool validate() const {
        if (sender_id.empty()) {
            return false;
        }
        if (auth_token.empty()) {
            return false;
        }
        if (encrypted_payload.empty()) {
            return false;
        }
        if (iv.size() != 32) {
            return false;
        }
        if (payload_size != static_cast<uint32_t>(encrypted_payload.size())) {
            return false;
        }
        if (timestamp == 0) {
            return false;
        }
        if (checksum.empty()) {
            return false;
        }
        return true;
    }

    // Prints concise packet metadata for debugging/demo output.
    void print_debug() const {
        std::cout << "[PACKET DEBUG]" << std::endl;
        std::cout << "Sender: " << sender_id << std::endl;
        std::cout << "Sequence Number: " << sequence_number << std::endl;
        std::cout << "Timestamp: " << timestamp << std::endl;
        std::cout << "Payload Size: " << payload_size << std::endl;
        std::cout << "Checksum Present: " << (!checksum.empty()) << "\n";
    }
};

// Backward-compatible alias used by older code paths.
using MediaPacket = Packet;

#endif  // PROTOCOL_PACKET_H
