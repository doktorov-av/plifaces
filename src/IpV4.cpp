//
// Created by DoktorovAlexander on 21.02.2026.
//

#include "IpV4.hpp"
#include "Errors.hpp"
#include <system_error>

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

PLIFACES_NAMESPACE_BEGIN

IpV4 IpV4::FromStringView(std::string_view str) {
    IpV4 ip{};
    char buff[sizeof(sockaddr_in)];
    if (inet_pton(AF_INET, str.data(), buff) == -1) {
        throw std::system_error(errno, std::system_category(), SystemErrorMessage());
    }
    std::memcpy(&ip.data, buff, sizeof(ip.data));
    return ip;
}

std::string IpV4::ToString() const {
    char buff[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &data, buff, sizeof(buff)) == nullptr) {
        throw std::system_error(errno, std::system_category(), SystemErrorMessage());
    }
    return { buff };
}

bool IpV4::operator==(const IpV4& rhs) const {
    return this->data == rhs.data;
}

PLIFACES_NAMESPACE_END