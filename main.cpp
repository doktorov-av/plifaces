#include <iostream>

#include "Netlink.hpp"
#include "Proc.hpp"

#include <cstring>
#include <net/if.h>

// 1. /proc/net/unix contains all the sockets and associated inode
// 2. /proc/net/tcp

template <class T>
void print_container(T&& C) {
    std::cout << "[ ";
    for (const auto& v : C) {
        std::cout << v << ' ';
    }
    std::cout << "]\n";
}

void print_addrs(const std::vector<NetAddress>& addr) {
    std::cout << "[ ";
    for (const auto& v : addr) {
        std::cout << v.GetIpV4String() << ':' << v.port << ' ';
    }
    std::cout << "]\n";
}

void test_get_interface_info() {

}

int test_plifaces() {
    auto fds = plifaces::ReadFileDescriptors(3461829);
    auto sockets = plifaces::ExtractSockets(std::move(fds));
    auto nodes = plifaces::ExtractSocketsNodes(sockets);
    auto addrs = plifaces::ExtractLocalAddress(nodes);

    print_container(fds);
    print_container(sockets);
    print_container(nodes);
    print_addrs(addrs);

    return 0;
}

int main()
{
    Netlink netlink;
    netlink.Open();
    netlink.RequestRouteDump();
    auto routes = netlink.ReceiveRouteDump();

    for(auto& r : routes) {
        if (r.HasGateway() && r.IsDefaultDestination()) {
            std::cout << "default via " << r.gateway << std::endl;
        }
    }

    return 0;
}

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <linux/if.h>

void print_interfaces() {
    struct ifaddrs* ifaddr;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
      can free list later.  */

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL;
         ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        /* Display interface name and family (including symbolic
          form of the latter for the common families).  */

        const bool isGlobal = ifa->ifa_flags & (IFF_LOWER_UP | IFF_UP);
        const bool isLoopback = ifa->ifa_flags & IFF_LOOPBACK;

        if (family == AF_INET && isGlobal && !isLoopback) {
            s = getnameinfo(ifa->ifa_addr,
                            (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                            host, NI_MAXHOST,
                            NULL, 0, NI_NUMERICHOST);

            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            printf("%s\t\taddress: <%s>\n", ifa->ifa_name, host);

        }
         }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}