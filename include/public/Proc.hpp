//
// Created by doktorov-av on 2/12/26.
//

#ifndef SHMEM_PROC_HPP
#define SHMEM_PROC_HPP

#include "../private/Directory.hpp"

namespace proc {
    std::filesystem::path MakeDescriptorsDir(int pid);
    [[nodiscard]] std::vector<std::string> ReadFileDescriptors(int pid);
    [[nodiscard]] std::vector<std::string> ExtractSockets(std::vector<std::string> fds);
    [[nodiscard]] std::vector<int> ExtractSocketsNodes(const std::vector<std::string>& sockets);
    [[nodiscard]] std::vector<std::string> ExtractLocalAddress(const std::vector<int>& inodes);
}// namespace proc

#endif//SHMEM_PROC_HPP