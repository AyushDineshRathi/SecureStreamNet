#include "packet.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

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

string serialize_packet(const Packet& packet) {
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

    cout << "[SERIALIZER] Packet serialized" << endl;
    return j.dump();
}

Packet deserialize_packet(const string& json_string) {
    const json j = json::parse(json_string);

    // Fail fast if required fields are missing.
    const vector<string> required_fields = {
        "sender_id",
        "auth_token",
        "timestamp",
        "sequence_number",
        "payload_size",
        "iv",
        "encrypted_payload",
        "checksum"};

    vector<string> missing_fields;
    for (const auto& field : required_fields) {
        if (!j.contains(field)) {
            missing_fields.push_back(field);
        }
    }

    if (!missing_fields.empty()) {
        string message = "Missing required JSON fields: ";
        for (size_t i = 0; i < missing_fields.size(); ++i) {
            message += missing_fields[i];
            if (i + 1 < missing_fields.size()) {
                message += ", ";
            }
        }
        throw runtime_error(message);
    }

    Packet packet{};

    packet.sender_id = j.at("sender_id").get<string>();
    packet.auth_token = j.at("auth_token").get<string>();
    packet.timestamp = j.at("timestamp").get<uint64_t>();
    packet.sequence_number = j.at("sequence_number").get<uint32_t>();
    packet.payload_size = j.at("payload_size").get<uint32_t>();
    packet.iv = j.at("iv").get<string>();
    packet.encrypted_payload = j.at("encrypted_payload").get<string>();
    packet.checksum = j.at("checksum").get<string>();

    cout << "[SERIALIZER] Packet deserialized" << endl;
    return packet;
}

// Backward-compatible wrappers for older call sites.
string serializePacket(const Packet& packet) { return serialize_packet(packet); }
Packet deserializePacket(const string& payload) { return deserialize_packet(payload); }
