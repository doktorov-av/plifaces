//
// Created by doktorov-av on 2/19/26.
//

#ifndef PLIFACES_IPV4_HPP
#define PLIFACES_IPV4_HPP
#include <array>
#include <cstdint>
#include <string>

PLIFACES_NAMESPACE_BEGIN

class IpV4 {
public:
    IpV4() = default;
    static IpV4 FromStringView(std::string_view str);
    [[nodiscard]] std::string ToString() const;
    bool operator==(const IpV4& rhs) const;

    uint32_t data = 0; // bytes representation of ipV4
};

PLIFACES_NAMESPACE_END

#endif//PLIFACES_IPV4_HPP
