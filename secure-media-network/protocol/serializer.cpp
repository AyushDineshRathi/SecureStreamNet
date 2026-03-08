#include "packet.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#elif __has_include("../include/nlohmann/json.hpp")
#include "../include/nlohmann/json.hpp"
#elif __has_include("../../include/nlohmann/json.hpp")
#include "../../include/nlohmann/json.hpp"
#else
#error "nlohmann/json.hpp not found. Add include path to the nlohmann single-header location."
#endif

using json = nlohmann::json;

std::string serialize_packet(const Packet& packet) {
    // Build a JSON object with the agreed packet schema.
    json j;

    j["sender_id"] = packet.sender_id;
    j["auth_token"] = packet.auth_token;
    j["timestamp"] = packet.timestamp;
    j["sequence_number"] = packet.sequence_number;
    j["payload_size"] = packet.payload_size;
    j["iv"] = packet.iv;
    j["encrypted_payload"] = packet.encrypted_payload;
    j["checksum"] = packet.checksum;

    std::cout << "[SERIALIZER] Packet serialized" << std::endl;
    return j.dump();
}

Packet deserialize_packet(const std::string& json_string) {
    const json j = json::parse(json_string);

    // Fail fast if required fields are missing.
    const std::vector<std::string> required_fields = {
        "sender_id",
        "auth_token",
        "timestamp",
        "sequence_number",
        "payload_size",
        "iv",
        "encrypted_payload",
        "checksum"};

    std::vector<std::string> missing_fields;
    for (const auto& field : required_fields) {
        if (!j.contains(field)) {
            missing_fields.push_back(field);
        }
    }

    if (!missing_fields.empty()) {
        std::string message = "Missing required JSON fields: ";
        for (size_t i = 0; i < missing_fields.size(); ++i) {
            message += missing_fields[i];
            if (i + 1 < missing_fields.size()) {
                message += ", ";
            }
        }
        throw std::runtime_error(message);
    }

    Packet packet{};

    packet.sender_id = j.at("sender_id").get<std::string>();
    packet.auth_token = j.at("auth_token").get<std::string>();
    packet.timestamp = j.at("timestamp").get<std::uint64_t>();
    packet.sequence_number = j.at("sequence_number").get<std::uint32_t>();
    packet.payload_size = j.at("payload_size").get<std::uint32_t>();
    packet.iv = j.at("iv").get<std::string>();
    packet.encrypted_payload = j.at("encrypted_payload").get<std::string>();
    packet.checksum = j.at("checksum").get<std::string>();

    std::cout << "[SERIALIZER] Packet deserialized" << std::endl;
    return packet;
}

// Backward-compatible wrappers for older call sites.
std::string serializePacket(const Packet& packet) { return serialize_packet(packet); }
Packet deserializePacket(const std::string& payload) { return deserialize_packet(payload); }
