//
// Created by doktorov-av on 17.02.2026.
//

#ifndef PLIFACES_NETLINK_HPP
#define PLIFACES_NETLINK_HPP

#include "Route.hpp"
#include <vector>


class Netlink {
public:
    Netlink() = default;

    [[nodiscard]] bool IsOpened() const { return m_SocketFd != -1; }
    void Open();
    void Close();
    void RequestRouteDump();
    [[nodiscard]] std::vector<Route> ReceiveRouteDump() const;

    ~Netlink();
protected:
    // throws system_error() with errMessage and errno and calls close() on m_SocketFd
    void Terminate(const char* errMessage);
private:
    static int RecvMessage(int sock, struct msghdr* msg, char** answer);
    static int Recv(int sock, struct msghdr* msg, int flags);
    static void ParseRattr(struct rtattr* tb[], int max, struct rtattr* rta, int len);
    static void PrintRoute(struct nlmsghdr* nl_header_answer);

    int m_SocketFd = -1;
};

#endif //PLIFACES_NETLINK_HPP
