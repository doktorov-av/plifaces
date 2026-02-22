//
// Created by doktorov-av on 2/19/26.
//

#ifndef PLIFACES_NETINTERFACE_HPP
#define PLIFACES_NETINTERFACE_HPP
#include "IpV4.hpp"
#include "Mac.hpp"
#include <vector>

PLIFACES_NAMESPACE_BEGIN

struct InterfaceAddress {
    std::vector<Mac> macs;
    std::vector<IpV4> ips;
};

PLIFACES_NAMESPACE_END

#endif//PLIFACES_NETINTERFACE_HPP
