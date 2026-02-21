//
// Created by doktorov-av on 2/22/26.
//

#include "Mac.hpp"

PLIFACES_NAMESPACE_BEGIN

std::string Mac::ToString() const {
    char buffer[18];
    snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x",
             data[0],
             data[1],
             data[2],
             data[3],
             data[4],
             data[5]);

    return {buffer};
}

PLIFACES_NAMESPACE_END