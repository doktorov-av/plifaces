#include <iostream>

#include "Proc.hpp"

// 1. /proc/net/unix contains all the sockets and associated inode
// 2. /proc/net/tcp

void print_container(auto&& C) {
    std::cout << "[ ";
    for (const auto& v : C) {
        std::cout << v << ' ';
    }
    std::cout << "]\n";
}

int main(int argc, char* argv[]) {
    auto fds = proc::ReadFileDescriptors(42428);
    auto sockets = proc::ExtractSockets(std::move(fds));
    auto nodes = proc::ExtractSocketsNodes(sockets);
    auto addrs = proc::ExtractLocalAddress(nodes);

    print_container(fds);
    print_container(sockets);
    print_container(nodes);
    print_container(addrs);
    return 0;
}