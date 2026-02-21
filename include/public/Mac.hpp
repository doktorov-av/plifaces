//
// Created by doktorov-av on 2/22/26.
//

#ifndef PLIFACES_MAC_HPP
#define PLIFACES_MAC_HPP
#include <array>
#include <cstdint>
#include <string>

PLIFACES_NAMESPACE_BEGIN

class Mac {
public:
    Mac() = default;
    [[nodiscard]] std::string ToString() const;

    std::array<unsigned char, 8> data;
};

PLIFACES_NAMESPACE_END

#endif//PLIFACES_MAC_HPP
