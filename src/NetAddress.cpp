//
// Created by doktorov-av on 17.02.2026.
//

#include "NetAddress.hpp"

#include <complex>
#include <sstream>

NetAddress::NetAddress(std::array<uint8_t, 4> parts) {
    for (int i = 0; i < parts.size(); i++) {
        const auto shift = i * sizeof(uint8_t);
        ip_v4 |= parts[i] << shift;
    }
}

std::string NetAddress::GetIpV4String() const {
    std::stringstream result;
    for (int shift = 0; shift < 4; ++shift) {
        constexpr auto mask = (1 << sizeof(uint8_t) * 8) - 1;
        const uint8_t part = ip_v4 & (mask << shift);
        result << static_cast<int>(part);
        if (shift < 3) {
            result << '.';
        }
    }
    return result.str();
}