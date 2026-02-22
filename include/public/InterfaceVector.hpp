//
// Created by DoktorovAlexander on 22.02.2026.
//

#ifndef PLIFACES_INTERFACEVECTOR_HPP
#define PLIFACES_INTERFACEVECTOR_HPP

#include "Interface.hpp"

#include <optional>
#include <vector>

PLIFACES_NAMESPACE_BEGIN

class InterfaceVector {
public:
    explicit InterfaceVector(std::vector<Interface> data);

    [[nodiscard]] const std::vector<Interface>& GetInterfaces() const;
    [[nodiscard]] std::optional<Interface> FindByIp(IpV4 ip) const;
private:
    std::vector<Interface> m_Data;
};

PLIFACES_NAMESPACE_END

#endif //PLIFACES_INTERFACEVECTOR_HPP
