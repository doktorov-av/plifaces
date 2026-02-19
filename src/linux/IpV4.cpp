//
// Created by doktorov-av on 2/19/26.
//

#include "IpV4.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <system_error>

PLIFACES_NAMESPACE_BEGIN

IpV4 IpV4::FromStringView(std::string_view str) {
    IpV4 ip{};
    char buff[sizeof(sockaddr_in)];
    if (inet_pton(AF_INET, str.data(), buff) == -1) {
        throw std::system_error(errno, std::system_category(), strerror(errno));
    }
    std::memcpy(&ip.data, buff, sizeof(ip.data));
    return ip;
}

std::string IpV4::ToString() const {
    char buff[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &data, buff, sizeof(buff)) == nullptr) {
        throw std::system_error(errno, std::system_category(), strerror(errno));
    }
    return { buff };
}

bool IpV4::operator==(const IpV4& rhs) const {
    return this->data == rhs.data;
}

PLIFACES_NAMESPACE_END