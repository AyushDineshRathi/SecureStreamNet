# Secure Media Network

This project is a C++17 demo for sending one encrypted media frame from a TCP client to a TCP server.

The client:
- connects to the local server
- encrypts a demo frame with AES-256-CBC
- hashes the encrypted payload with SHA-256
- serializes the packet as JSON
- sends it over TCP

The server:
- accepts one client connection
- receives the JSON packet
- validates the packet structure
- verifies the checksum
- decrypts the payload
- prints the recovered frame

## Project Structure

- `secure-media-network/client/`: client entry point
- `secure-media-network/server/`: server entry point
- `secure-media-network/protocol/`: packet model and JSON serializer
- `secure-media-network/security/`: AES and SHA helpers
- `secure-media-network/network/`: socket utilities
- `secure-media-network/common/`: shared configuration
- `include/nlohmann/json.hpp`: bundled JSON dependency

## Current Demo Configuration

Shared settings live in [`secure-media-network/common/config.h`](secure-media-network/common/config.h):
- host: `127.0.0.1`
- port: `9000`
- backlog: `5`
- demo sender id: `device_01`
- demo auth token: `demo_token`
- demo payload: `FRAME_1_DATA`

## Prerequisites

On this Windows setup, the project builds with:
- `mingw32-make`
- MSYS2 MinGW64 `g++`
- OpenSSL libraries available under `C:\msys64\mingw64\bin`

## Build

From PowerShell:

```powershell
cd "c:\Users\Ayush R\Desktop\Notes\AI_ML\Projects\SecureStreamNet\secure-media-network"
mingw32-make clean
mingw32-make
```

## Run

Open PowerShell window 1:

```powershell
cd "c:\Users\Ayush R\Desktop\Notes\AI_ML\Projects\SecureStreamNet\secure-media-network"
$env:PATH="C:\msys64\mingw64\bin;$env:PATH"
.\server_app.exe
```

Open PowerShell window 2:

```powershell
cd "c:\Users\Ayush R\Desktop\Notes\AI_ML\Projects\SecureStreamNet\secure-media-network"
$env:PATH="C:\msys64\mingw64\bin;$env:PATH"
.\client_app.exe
```

Expected result on the server:

```text
[SERVER] Listening on 127.0.0.1:9000...
[SERVER] Client connected
[SERVER] Packet JSON received
[SERVER] Packet deserialized
[SERVER] Packet validated
[SERVER] Checksum computed
[SERVER] Integrity check passed
[SERVER] Payload decrypted
[SERVER] Frame received: FRAME_1_DATA
```

## Notes

- The AES key is hardcoded for demo purposes only.
- The current server handles a single client and then exits.
- `sha256.cpp` still uses deprecated OpenSSL SHA APIs; it builds successfully but can be modernized later to EVP-based hashing.
