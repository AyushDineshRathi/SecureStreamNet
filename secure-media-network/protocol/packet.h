#ifndef PROTOCOL_PACKET_H
#define PROTOCOL_PACKET_H

#include <cstdint>
#include <string>

// Represents one logical media packet sent between client and server.
struct MediaPacket {
    std::uint64_t sequence_id;
    std::string media_type;
    std::string encrypted_payload_b64;
    std::string sha256_hex;
};

#endif  // PROTOCOL_PACKET_H
