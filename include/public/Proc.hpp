//
// Created by doktorov-av on 2/12/26.
//

#ifndef PLIFACES_PROC_HPP
#define PLIFACES_PROC_HPP

#include "NetAddress.hpp"

#include <filesystem>
#include <vector>

PLIFACES_NAMESPACE_BEGIN

std::filesystem::path MakeDescriptorsDir(int pid);
[[nodiscard]] std::vector<std::string> ReadFileDescriptors(int pid);
[[nodiscard]] std::vector<std::string> ExtractSockets(std::vector<std::string> fds);
[[nodiscard]] std::vector<int> ExtractSocketsNodes(const std::vector<std::string>& sockets);
[[nodiscard]] std::vector<NetAddress> ExtractLocalAddress(const std::vector<int>& inodes);

PLIFACES_NAMESPACE_END

#endif//PLIFACES_PROC_HPP