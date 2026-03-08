#ifndef SECURITY_AES_ENCRYPT_H
#define SECURITY_AES_ENCRYPT_H

#include <string>

// Generates a random 16-byte IV (raw bytes in std::string).
std::string generate_random_iv();

// Encrypts plaintext using AES-256-CBC and returns base64 ciphertext.
std::string encrypt_data(
    const std::string& plaintext,
    const std::string& key,
    const std::string& iv);

// Decrypts base64 ciphertext using AES-256-CBC and returns plaintext.
std::string decrypt_data(
    const std::string& ciphertext,
    const std::string& key,
    const std::string& iv);

#endif  // SECURITY_AES_ENCRYPT_H
