#ifndef PROTOCOL_SERIALIZER_H
#define PROTOCOL_SERIALIZER_H

#include <string>
#include "packet.h"

std::string serialize_packet(const Packet& packet);
Packet deserialize_packet(const std::string& json_string);

// backward compatibility
std::string serializePacket(const Packet& packet);
Packet deserializePacket(const std::string& payload);

#endif