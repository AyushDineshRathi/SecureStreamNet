#include "sha256.h"

#include <openssl/sha.h>

#include <iostream>

std::string sha256Hex(const std::vector<std::uint8_t>& input) {
    // TODO(phase-2): Implement SHA-256 digest computation and hex formatting.
    (void)input;
    std::cout << "[SECURITY] sha256Hex skeleton called." << std::endl;
    return "";
}

bool verifySha256Hex(const std::vector<std::uint8_t>& input, const std::string& expected_hex) {
    // TODO(phase-2): Compare computed digest against expected hash.
    (void)input;
    (void)expected_hex;
    std::cout << "[SECURITY] verifySha256Hex skeleton called." << std::endl;
    return false;
}
