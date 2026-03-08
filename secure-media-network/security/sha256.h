#ifndef SECURITY_SHA256_H
#define SECURITY_SHA256_H

#include <string>

// Computes SHA-256 for input data and returns a lowercase hex string.
std::string compute_sha256(const std::string& data);

#endif  // SECURITY_SHA256_H
