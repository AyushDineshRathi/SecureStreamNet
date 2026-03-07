#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include <string>

namespace config {

inline constexpr int kServerPort = 5555;
inline constexpr int kServerBacklog = 5;
inline const std::string kServerHost = "127.0.0.1";

// TODO(phase-2): Replace demo key/iv placeholders with secure key management.
inline const std::string kDemoAes256KeyHex = "";
inline const std::string kDemoAesIvHex = "";

}  // namespace config

#endif  // COMMON_CONFIG_H
