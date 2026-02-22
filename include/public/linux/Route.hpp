//
// Created by doktorov-av on 17.02.2026.
//

#ifndef PLIFACES_ROUTE_HPP
#define PLIFACES_ROUTE_HPP
#include <optional>
#include <string>

struct Route {
    explicit Route(struct nlmsghdr* header);

    bool IsDefaultDestination() const { return destination.empty(); }
    bool HasGateway() const { return !gateway.empty(); }

    std::string destination;
    std::string gateway;
    std::string ifname;
    std::string source;
};

#endif //PLIFACES_ROUTE_HPP
