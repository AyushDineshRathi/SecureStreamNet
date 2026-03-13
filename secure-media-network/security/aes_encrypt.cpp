#include "aes_encrypt.h"

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

namespace {
constexpr size_t kAes256KeySize = 32;
constexpr size_t kAesCbcIvSize = 16;

runtime_error make_openssl_error(const string& prefix) {
    const unsigned long code = ERR_get_error();
    if (code == 0UL) {
        return runtime_error(prefix);
    }

    char buffer[256];
    ERR_error_string_n(code, buffer, sizeof(buffer));
    return runtime_error(prefix + ": " + buffer);
}

string base64_encode(const unsigned char* data, int len) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    if (b64 == nullptr || mem == nullptr) {
        if (b64 != nullptr) {
            BIO_free(b64);
        }
        if (mem != nullptr) {
            BIO_free(mem);
        }
        throw make_openssl_error("Failed to create BIO for base64 encode");
    }

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO* chain = BIO_push(b64, mem);

    if (BIO_write(chain, data, len) <= 0) {
        BIO_free_all(chain);
        throw make_openssl_error("BIO_write failed during base64 encode");
    }
    if (BIO_flush(chain) != 1) {
        BIO_free_all(chain);
        throw make_openssl_error("BIO_flush failed during base64 encode");
    }

    BUF_MEM* mem_ptr = nullptr;
    BIO_get_mem_ptr(chain, &mem_ptr);
    if (mem_ptr == nullptr || mem_ptr->data == nullptr || mem_ptr->length == 0) {
        BIO_free_all(chain);
        return "";
    }

    const string encoded(mem_ptr->data, mem_ptr->length);
    BIO_free_all(chain);
    return encoded;
}

vector<unsigned char> base64_decode(const string& input) {
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new_mem_buf(input.data(), static_cast<int>(input.size()));
    if (b64 == nullptr || mem == nullptr) {
        if (b64 != nullptr) {
            BIO_free(b64);
        }
        if (mem != nullptr) {
            BIO_free(mem);
        }
        throw make_openssl_error("Failed to create BIO for base64 decode");
    }

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO* chain = BIO_push(b64, mem);

    vector<unsigned char> decoded(input.size(), 0);
    const int out_len = BIO_read(chain, decoded.data(), static_cast<int>(decoded.size()));
    if (out_len < 0) {
        BIO_free_all(chain);
        throw make_openssl_error("BIO_read failed during base64 decode");
    }

    decoded.resize(static_cast<size_t>(out_len));
    BIO_free_all(chain);
    return decoded;
}

void validate_key_iv(const string& key, const string& iv) {
    if (key.size() != kAes256KeySize) {
        throw invalid_argument("AES-256 key must be exactly 32 bytes");
    }
    if (iv.size() != kAesCbcIvSize) {
        throw invalid_argument("AES-CBC IV must be exactly 16 bytes");
    }
}
}  // namespace

string generate_random_iv() {
    string iv(kAesCbcIvSize, '\0');
    if (RAND_bytes(reinterpret_cast<unsigned char*>(&iv[0]), static_cast<int>(iv.size())) != 1) {
        throw make_openssl_error("RAND_bytes failed for IV generation");
    }
    return iv;
}

string encrypt_data(
    const string& plaintext,
    const string& key,
    const string& iv) {
    validate_key_iv(key, iv);
    cout << "[SECURITY] Encrypting payload" << endl;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) {
        throw make_openssl_error("EVP_CIPHER_CTX_new failed");
    }

    vector<unsigned char> ciphertext(
        plaintext.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int out_len_1 = 0;
    int out_len_2 = 0;

    if (EVP_EncryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            nullptr,
            reinterpret_cast<const unsigned char*>(key.data()),
            reinterpret_cast<const unsigned char*>(iv.data())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw make_openssl_error("EVP_EncryptInit_ex failed");
    }

    if (EVP_EncryptUpdate(
            ctx,
            ciphertext.data(),
            &out_len_1,
            reinterpret_cast<const unsigned char*>(plaintext.data()),
            static_cast<int>(plaintext.size())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw make_openssl_error("EVP_EncryptUpdate failed");
    }

    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + out_len_1, &out_len_2) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw make_openssl_error("EVP_EncryptFinal_ex failed");
    }

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(static_cast<size_t>(out_len_1 + out_len_2));
    return base64_encode(ciphertext.data(), static_cast<int>(ciphertext.size()));
}

string decrypt_data(
    const string& ciphertext,
    const string& key,
    const string& iv) {
    validate_key_iv(key, iv);
    cout << "[SECURITY] Decrypting payload" << endl;

    const vector<unsigned char> decoded_ciphertext = base64_decode(ciphertext);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) {
        throw make_openssl_error("EVP_CIPHER_CTX_new failed");
    }

    vector<unsigned char> plaintext(
        decoded_ciphertext.size() + EVP_MAX_BLOCK_LENGTH, 0);
    int out_len_1 = 0;
    int out_len_2 = 0;

    if (EVP_DecryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            nullptr,
            reinterpret_cast<const unsigned char*>(key.data()),
            reinterpret_cast<const unsigned char*>(iv.data())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw make_openssl_error("EVP_DecryptInit_ex failed");
    }

    if (EVP_DecryptUpdate(
            ctx,
            plaintext.data(),
            &out_len_1,
            decoded_ciphertext.data(),
            static_cast<int>(decoded_ciphertext.size())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw make_openssl_error("EVP_DecryptUpdate failed");
    }

    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + out_len_1, &out_len_2) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw make_openssl_error("EVP_DecryptFinal_ex failed");
    }

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(static_cast<size_t>(out_len_1 + out_len_2));
    return string(
        reinterpret_cast<const char*>(plaintext.data()),
        plaintext.size());
}
