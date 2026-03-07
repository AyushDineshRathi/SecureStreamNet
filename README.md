# Secure Media Network (Skeleton)

This project is an initial C++17 prototype scaffold for secure media packet transport over TCP.

## Architecture

- `client/`: client entry point responsible for building and sending secure packets.
- `server/`: server entry point responsible for receiving, decrypting, and validating packets.
- `protocol/`: packet model and JSON serialization/deserialization.
- `security/`: cryptographic helpers for AES-256 encryption and SHA-256 hashing.
- `network/`: POSIX TCP socket wrappers for client/server communication.
- `common/`: shared configuration constants.

## Notes

- Current files are skeleton-only for demonstration.
- Core networking and cryptographic logic are intentionally left as TODOs.
- Logging statements are included to show intended control flow in later phases.
