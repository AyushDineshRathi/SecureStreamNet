#include "sha256.h"

#include <openssl/sha.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;

string compute_sha256(const string& data) {
    cout << "[SECURITY] Computing SHA256 checksum" << endl;

    SHA256_CTX ctx;
    unsigned char digest[SHA256_DIGEST_LENGTH];

    if (SHA256_Init(&ctx) != 1) {
        throw runtime_error("SHA256_Init failed");
    }

    if (SHA256_Update(
            &ctx,
            reinterpret_cast<const unsigned char*>(data.data()),
            data.size()) != 1) {
        throw runtime_error("SHA256_Update failed");
    }

    if (SHA256_Final(digest, &ctx) != 1) {
        throw runtime_error("SHA256_Final failed");
    }

    ostringstream hex_stream;
    hex_stream << hex << setfill('0');
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        hex_stream << setw(2) << static_cast<unsigned int>(digest[i]);
    }

    return hex_stream.str();
}
