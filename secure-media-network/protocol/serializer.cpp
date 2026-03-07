#include "packet.h"

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// TODO(phase-2): Move declarations to serializer.h once protocol API is finalized.
std::string serializePacket(const MediaPacket& packet) {
    // TODO(phase-2): Implement full JSON schema validation.
    json j;
    j["sequence_id"] = packet.sequence_id;
    j["media_type"] = packet.media_type;
    j["encrypted_payload_b64"] = packet.encrypted_payload_b64;
    j["sha256_hex"] = packet.sha256_hex;
    return j.dump();
}

MediaPacket deserializePacket(const std::string& payload) {
    // TODO(phase-2): Add robust parsing error handling.
    const json j = json::parse(payload);
    MediaPacket packet{};
    packet.sequence_id = j.value("sequence_id", 0ULL);
    packet.media_type = j.value("media_type", "");
    packet.encrypted_payload_b64 = j.value("encrypted_payload_b64", "");
    packet.sha256_hex = j.value("sha256_hex", "");
    return packet;
}
