#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include <string>

namespace config {

inline constexpr int kServerPort = 9000;
inline constexpr int kServerBacklog = 5;
inline const std::string kServerHost = "127.0.0.1";

// Demo-only values used by the sample client/server flow.
inline const std::string kDemoAes256Key = "0123456789abcdef0123456789abcdef";
inline const std::string kDemoSenderId = "device_01";
inline const std::string kDemoAuthToken = "demo_token";
inline const std::string kDemoFramePayload = "FRAME_1_DATA";

}  // namespace config

#endif  // COMMON_CONFIG_H
