#include "aes_encrypt.h"

#include <openssl/evp.h>

#include <iostream>

std::vector<std::uint8_t> encryptAes256(
    const std::vector<std::uint8_t>& plaintext,
    const std::vector<std::uint8_t>& key,
    const std::vector<std::uint8_t>& iv) {
    // TODO(phase-2): Implement AES-256 encryption with EVP APIs.
    (void)plaintext;
    (void)key;
    (void)iv;
    std::cout << "[SECURITY] encryptAes256 skeleton called." << std::endl;
    return {};
}

std::vector<std::uint8_t> decryptAes256(
    const std::vector<std::uint8_t>& ciphertext,
    const std::vector<std::uint8_t>& key,
    const std::vector<std::uint8_t>& iv) {
    // TODO(phase-2): Implement AES-256 decryption with EVP APIs.
    (void)ciphertext;
    (void)key;
    (void)iv;
    std::cout << "[SECURITY] decryptAes256 skeleton called." << std::endl;
    return {};
}
