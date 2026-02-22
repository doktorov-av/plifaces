//
// Created by doktorov-av on 17.02.2026.
//

#ifndef PLIFACES_NETADDRESS_HPP
#define PLIFACES_NETADDRESS_HPP
#include <array>
#include <cstdint>
#include <string>
#include <string_view>

struct NetAddress {
    uint16_t port = 0;
    uint32_t ip_v4 = 0;

    NetAddress() = default;
    explicit NetAddress(std::array<uint8_t, 4> parts);

    template <class I>
    static NetAddress FromParts(const std::array<I, 4>& parts) {
        std::array<uint8_t, 4> byteParts{};
        for (int i = 0; i < 4; i++) {
            byteParts[i] = static_cast<uint8_t>(parts[i]);
        }
        return NetAddress(byteParts);
    }

    [[nodiscard]] std::string GetIpV4String() const;
};

#endif //PLIFACES_NETADDRESS_HPP
