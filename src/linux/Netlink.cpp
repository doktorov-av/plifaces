//
// Created by doktorov-av on 17.02.2026.
//

#include <../include/public/linux/Netlink.hpp>

#include "../../include/public/linux/Route.hpp"

#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <ctime>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <stdexcept>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>
#include <vector>

void Netlink::Open() {
    Close();

    m_SocketFd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (m_SocketFd < 0) {
        Terminate("Failed to open netlink socket");
    }

    sockaddr_nl saddr{
            .nl_family = AF_NETLINK,
            .nl_pid = static_cast<uint32_t>(getpid()),
    };

    if (bind(m_SocketFd, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0) {
        Terminate("Failed to bind to netlink socket");
    }
}

void Netlink::Close() {
    if (m_SocketFd != -1) {
        close(m_SocketFd);
        m_SocketFd = -1;
    }
}

void Netlink::RequestRouteDump() {
    if (!IsOpened()) {
        throw std::runtime_error("Can't request route dump on closed socket");
    }

    struct {
        struct nlmsghdr nlh;
        struct rtmsg rtm;
    } nl_request;

    nl_request.nlh.nlmsg_type = RTM_GETROUTE;
    nl_request.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nl_request.nlh.nlmsg_len = sizeof(nl_request);
    nl_request.nlh.nlmsg_seq = time(nullptr);
    nl_request.rtm.rtm_family = AF_INET;

    if (send(m_SocketFd, &nl_request, sizeof(nl_request), 0) == -1) {
        Terminate("Failed to send request to netlink socket");
    }
}

void Netlink::ParseRattr(struct rtattr* tb[], int max, struct rtattr* rta, int len) {
    memset(tb, 0, sizeof(struct rtattr*) * (max + 1));

    while (RTA_OK(rta, len)) {
        if (rta->rta_type <= max) {
            tb[rta->rta_type] = rta;
        }

        rta = RTA_NEXT(rta, len);
    }
}

int Netlink::Recv(int sock, msghdr* msg, int flags) {
    int len;

    do {
        len = recvmsg(sock, msg, flags);
    } while (len < 0 && (errno == EINTR || errno == EAGAIN));

    if (len < 0) {
        perror("Netlink receive failed");
        return -errno;
    }

    if (len == 0) {
        perror("EOF on netlink");
        return -ENODATA;
    }

    return len;
}

int Netlink::RecvMessage(int sock, msghdr* msg, char** answer) {
    iovec* iov = msg->msg_iov;
    iov->iov_base = nullptr;
    iov->iov_len = 0;

    int len = Recv(sock, msg, MSG_PEEK | MSG_TRUNC);
    if (len < 0) {
        return len;
    }

    const auto buf = static_cast<char*>(malloc(len));
    if (!buf) {
        perror("malloc failed");
        return -ENOMEM;
    }

    iov->iov_base = buf;
    iov->iov_len = len;

    len = Recv(sock, msg, 0);
    if (len < 0) {
        free(buf);
        return len;
    }

    *answer = buf;
    return len;
}

void Netlink::PrintRoute(nlmsghdr* nl_header_answer) {
    Route r(nl_header_answer);
}

std::vector<Route> Netlink::ReceiveRouteDump() const {
    if (!IsOpened()) {
        throw std::runtime_error("Can't receive route dump on closed socket");
    }

    sockaddr_nl nladdr{};
    iovec iov{};
    msghdr msg = {
            .msg_name = &nladdr,
            .msg_namelen = sizeof(nladdr),
            .msg_iov = &iov,
            .msg_iovlen = 1,
    };

    std::vector<Route> routes;
    char* buf;
    int dump_intr = 0;
    int status = RecvMessage(m_SocketFd, &msg, &buf);

    struct nlmsghdr* h = reinterpret_cast<struct nlmsghdr*>(buf);
    int msglen = status;

    while (NLMSG_OK(h, msglen)) {
        if (h->nlmsg_flags & NLM_F_DUMP_INTR) {
            fprintf(stderr, "Dump was interrupted\n");
            free(buf);
            return routes;
        }

        if (nladdr.nl_pid != 0) {
            continue;
        }

        if (h->nlmsg_type == NLMSG_ERROR) {
            perror("netlink reported error");
            free(buf);
        }

        routes.emplace_back(Route(h));
        h = NLMSG_NEXT(h, msglen);
    }
    free(buf);

    return routes;
}
Netlink::~Netlink() {
    Close();
}

void Netlink::Terminate(const char* errMessage) {
    Close();
    std::error_code ec(errno, std::system_category());
    throw std::system_error(ec, errMessage);
}
