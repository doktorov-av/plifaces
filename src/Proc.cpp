//
// Created by doktorov-av on 2/12/26.
//

#include "Proc.hpp"
#include "Directory.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>

std::filesystem::path proc::MakeDescriptorsDir(int pid) {
    std::stringstream ss;
    ss << "/proc/" << pid << "/fd";
    return ss.str();
}

std::vector<std::string> proc::ReadFileDescriptors(int pid) {
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

std::vector<std::string> proc::ExtractSockets(std::vector<std::string> fds) {
    auto end = std::remove_if(fds.begin(), fds.end(), [](const std::string& fdLink) {
                   return fdLink.find("socket:") != 0;
               });
    return {fds.begin(), end};
}

std::vector<int> proc::ExtractSocketsNodes(const std::vector<std::string>& sockets) {
    std::vector<int> nodes(sockets.size());
    std::transform(sockets.begin(), sockets.end(), nodes.begin(), [](const std::string& str) {
        const auto startPos = str.find('[');
        const auto endPos = str.find(']', startPos);
        const auto iNode = str.substr(startPos + 1, endPos - startPos - 1);
        return stoi(iNode);
    });
    return nodes;
}

std::vector<NetAddress> proc::ExtractLocalAddress(const std::vector<int>& inodes) {
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

        auto addr = NetAddress::FromParts(std::array<unsigned int, 4>{ local_addr[0], local_addr[1], local_addr[2], local_addr[3] });
        addr.port = local_port;

        result.emplace_back(std::move(addr));
    }

    fclose(tcpFile);
    return result;
}
