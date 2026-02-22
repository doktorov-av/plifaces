//
// Created by doktorov-av on 17.02.2026.
//

// partially copied from https://olegkutkov.me/2019/03/24/getting-linux-routing-table-using-netlink/

#include "../../include/public/linux/Route.hpp"

#include <arpa/inet.h>
#include <bits/socket.h>
#include <cstdio>
#include <cstring>
#include <linux/rtnetlink.h>
#include <net/if.h>


static void parse_rattr(struct rtattr* tb[], int max, struct rtattr* rta, int len) {
    memset(tb, 0, sizeof(struct rtattr*) * (max + 1));

    while (RTA_OK(rta, len)) {
        if (rta->rta_type <= max) {
            tb[rta->rta_type] = rta;
        }

        rta = RTA_NEXT(rta, len);
    }
}

static int rtm_get_table(struct rtmsg* r, rtattr** tb) {
    int table = r->rtm_table;

    if (tb[RTA_TABLE]) {
        table = *static_cast<__u32*>(RTA_DATA(tb[RTA_TABLE]));
    }
    return table;
}

Route::Route(nlmsghdr* header) {
    const auto r = static_cast<rtmsg*>(NLMSG_DATA(header));
    int len = header->nlmsg_len;
    rtattr* tb[RTA_MAX + 1];
    char buf[256];

    len -= NLMSG_LENGTH(sizeof(*r));
    if (len < 0) {
        perror("Wrong message length");
        return;
    }

    parse_rattr(tb, RTA_MAX, RTM_RTA(r), len);
    int table = rtm_get_table(r, tb);

    if (r->rtm_family != AF_INET && table != RT_TABLE_MAIN) {
        return;
    }

    if (tb[RTA_DST]) {
        if ((r->rtm_dst_len != 24) && (r->rtm_dst_len != 16)) {
            return;
        }

        std::string ip = inet_ntop(r->rtm_family, RTA_DATA(tb[RTA_DST]), buf, sizeof(buf));
        destination = std::move(ip) + "/" + std::to_string(r->rtm_dst_len);
    } else if (r->rtm_dst_len) {
        destination = std::string("0/") + std::to_string(r->rtm_dst_len);
    }
    // else {
    // route destination is default
    // }

    if (tb[RTA_GATEWAY]) {
        gateway = inet_ntop(r->rtm_family, RTA_DATA(tb[RTA_GATEWAY]), buf, sizeof(buf));
    }

    if (tb[RTA_OIF]) {
        char if_nam_buf[IF_NAMESIZE];
        int ifidx = *(__u32*)RTA_DATA(tb[RTA_OIF]);
        ifname = if_indextoname(ifidx, if_nam_buf);
    }

    if (tb[RTA_SRC]) {
        source = inet_ntop(r->rtm_family, RTA_DATA(tb[RTA_SRC]), buf, sizeof(buf));
    }
}