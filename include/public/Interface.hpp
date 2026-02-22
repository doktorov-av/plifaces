//
// Created by DoktorovAlexander on 22.02.2026.
//

#ifndef PLIFACES_INTERFACE_HPP
#define PLIFACES_INTERFACE_HPP
#include "InterfaceAddress.hpp"
#include "Types.hpp"

PLIFACES_NAMESPACE_BEGIN

struct Interface {
    string_type name;
    InterfaceAddress address;
};

PLIFACES_NAMESPACE_END

#endif //PLIFACES_INTERFACE_HPP
