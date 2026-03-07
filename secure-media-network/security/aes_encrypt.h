#ifndef SECURITY_AES_ENCRYPT_H
#define SECURITY_AES_ENCRYPT_H

#include <cstdint>
#include <string>
#include <vector>

// Encrypts plaintext using AES-256.
std::vector<std::uint8_t> encryptAes256(
    const std::vector<std::uint8_t>& plaintext,
    const std::vector<std::uint8_t>& key,
    const std::vector<std::uint8_t>& iv);

// Decrypts ciphertext using AES-256.
std::vector<std::uint8_t> decryptAes256(
    const std::vector<std::uint8_t>& ciphertext,
    const std::vector<std::uint8_t>& key,
    const std::vector<std::uint8_t>& iv);

#endif  // SECURITY_AES_ENCRYPT_H
