//
// Created by DoktorovAlexander on 22.02.2026.
//

#include "InterfaceVector.hpp"

PLIFACES_NAMESPACE_BEGIN


InterfaceVector::InterfaceVector(std::vector<Interface> data) : m_Data(std::move(data)) {
}

const std::vector<Interface>& InterfaceVector::GetInterfaces() const {
    return m_Data;
}

std::optional<Interface> InterfaceVector::FindByIp(IpV4 ip) const {
    auto it = std::find_if(m_Data.begin(), m_Data.end(), [&](const auto& iface) {
        auto& ips = iface.address.ips;
        return std::find_if(ips.begin(), ips.end(), [&](const auto& Ip) {
                   return Ip == ip;
               }) != ips.end();
    });
    if (it != m_Data.end()) {
        return *it;
    }
    return std::nullopt;
}
PLIFACES_NAMESPACE_END