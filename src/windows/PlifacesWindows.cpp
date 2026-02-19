//
// Created by doktorov-av on 2/19/26.
//

#include "Plifaces.hpp"

PLIFACES_NAMESPACE_BEGIN

std::vector<NetInterface> GetInterfaceMacByIpV4(std::string_view ipV4) {
    std::vector<NetInterface> vec;
    vec.emplace_back(NetInterface{});
    vec.emplace_back(NetInterface{});
    vec.emplace_back(NetInterface{});
    vec.emplace_back(NetInterface{});
    return vec;
}

PLIFACES_NAMESPACE_END