#ifndef SECURITY_SHA256_H
#define SECURITY_SHA256_H

#include <cstdint>
#include <string>
#include <vector>

// Generates SHA-256 digest in hexadecimal format.
std::string sha256Hex(const std::vector<std::uint8_t>& input);

// Verifies the digest against expected hex hash.
bool verifySha256Hex(const std::vector<std::uint8_t>& input, const std::string& expected_hex);

#endif  // SECURITY_SHA256_H
