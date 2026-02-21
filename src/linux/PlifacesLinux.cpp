//
// Created by doktorov-av on 2/12/26.
//

#include "Directory.hpp"
#include "NetAddress.hpp"
#include "Plifaces.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>
#include <iostream>
#include <netdb.h>
#include <linux/if_packet.h>

PLIFACES_NAMESPACE_BEGIN

std::filesystem::path MakeDescriptorsDir(int pid) {
    std::stringstream ss;
    ss << "/proc/" << pid << "/fd";
    return ss.str();
}

std::vector<std::string> ReadFileDescriptors(int pid) {
    std::vector<std::string> res;

    Directory dir = {};
    try {
        dir.Open(MakeDescriptorsDir(pid));
    } catch (std::runtime_error&) {
        std::cout << "no such pid\n";
        return {};
    }

    dir.ForEachEntry([&res](DirEntry&& entry) {
        if (auto link = entry.ReadlinkOptional()) {
            res.emplace_back(std::move(link.value()));
        }
    });

    return res;
}

std::vector<std::string> ExtractSockets(std::vector<std::string> fds) {
    auto end = std::remove_if(fds.begin(), fds.end(), [](const std::string& fdLink) {
        return fdLink.find("socket:") != 0;
    });
    return {fds.begin(), end};
}

std::vector<int> ExtractSocketsNodes(const std::vector<std::string>& sockets) {
    std::vector<int> nodes(sockets.size());
    std::transform(sockets.begin(), sockets.end(), nodes.begin(), [](const std::string& str) {
        const auto startPos = str.find('[');
        const auto endPos = str.find(']', startPos);
        const auto iNode = str.substr(startPos + 1, endPos - startPos - 1);
        return stoi(iNode);
    });
    return nodes;
}

std::vector<NetAddress> ExtractLocalAddress(const std::vector<int>& inodes) {
    FILE* tcpFile = fopen("/proc/net/tcp", "r");
    if (!tcpFile) {
        throw std::runtime_error(strerror(errno));
    }

    char line[512];
    std::vector<NetAddress> result;
    result.reserve(inodes.size());

    fgets(line, sizeof(line), tcpFile);

    while (fgets(line, sizeof(line), tcpFile)) {
        unsigned int local_addr[4], rem_addr[8];
        unsigned int local_port, rem_port, state, uid, inode;

        sscanf(line,
               "%*d: %02X%02X%02X%02X:%04X %02X%02X%02X%02X:%04X %02X %*x:%*x %*x:%*x %*x %d %*d %d",
               &local_addr[0], &local_addr[1], &local_addr[2], &local_addr[3],
               &local_port,
               &rem_addr[0], &rem_addr[1], &rem_addr[2], &rem_addr[3],
               &rem_port, &state, &uid, &inode);

        if (auto it = std::find(inodes.begin(), inodes.end(), inode); it == inodes.end()) {
            continue;
        }

        auto addr = NetAddress::FromParts(std::array<unsigned int, 4>{local_addr[0], local_addr[1], local_addr[2], local_addr[3]});
        addr.port = local_port;

        result.emplace_back(std::move(addr));
    }

    fclose(tcpFile);
    return result;
}

std::unordered_map<std::string, InterfaceAddress> GetInterfaces() {
    ifaddrs* interfaceAddresses = nullptr;
    if (getifaddrs(&interfaceAddresses) == -1) {
        throw std::system_error(errno, std::system_category(), strerror(errno));
    }

    std::unordered_map<std::string, InterfaceAddress> interfaces{};
    for (ifaddrs* ifa = interfaceAddresses; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
            continue;
        }

        auto name = std::string(ifa->ifa_name);
        auto& interface = interfaces[name];

        const auto family = ifa->ifa_addr->sa_family;

        // process AF_INET address
        if (family == AF_INET) {
            const auto sin = reinterpret_cast<sockaddr_in*>(ifa->ifa_addr);
            if (!sin) {
                continue;
            }

            auto& ip = interface.ips.emplace_back();
            ip.data = sin->sin_addr.s_addr;
        } else if (family == AF_PACKET) {
            const auto paddr = reinterpret_cast<sockaddr_ll*>(ifa->ifa_addr);
            if (!paddr) {
                continue;
            }

            auto& mac = interface.macs.emplace_back();
            std::memcpy(mac.data.data(), paddr->sll_addr, paddr->sll_halen);
        }
    }

    freeifaddrs(interfaceAddresses);
    return interfaces;
}

PLIFACES_NAMESPACE_END