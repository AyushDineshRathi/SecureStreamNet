#include "sha256.h"

#include <openssl/sha.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string compute_sha256(const std::string& data) {
    std::cout << "[SECURITY] Computing SHA256 checksum" << std::endl;

    SHA256_CTX ctx;
    unsigned char digest[SHA256_DIGEST_LENGTH];

    if (SHA256_Init(&ctx) != 1) {
        throw std::runtime_error("SHA256_Init failed");
    }

    if (SHA256_Update(
            &ctx,
            reinterpret_cast<const unsigned char*>(data.data()),
            data.size()) != 1) {
        throw std::runtime_error("SHA256_Update failed");
    }

    if (SHA256_Final(digest, &ctx) != 1) {
        throw std::runtime_error("SHA256_Final failed");
    }

    std::ostringstream hex_stream;
    hex_stream << std::hex << std::setfill('0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hex_stream << std::setw(2) << static_cast<unsigned int>(digest[i]);
    }

    return hex_stream.str();
}
