//
// Created by doktorov-av on 2/19/26.
//

#include "Errors.hpp"
#include "Plifaces.hpp"

#include <WinSock2.h>
#include <cassert>
#include <iostream>
#include <iphlpapi.h>
#include <unordered_map>
#include <ws2tcpip.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

PLIFACES_NAMESPACE_BEGIN

static std::string wstring_to_utf8(const std::wstring& wideString) {
    if (wideString.empty()) return {};

    const int size_needed = WideCharToMultiByte(
            CP_UTF8,
            0,
            wideString.c_str(),
            static_cast<int>(wideString.size()),
            nullptr,
            0,
            nullptr,
            nullptr);

    std::string converted(size_needed, 0);

    WideCharToMultiByte(
            CP_UTF8,
            0,
            wideString.c_str(),
            static_cast<int>(wideString.size()),
            converted.data(),
            size_needed,
            nullptr,
            nullptr);
    return converted;
}

InterfaceVector GetInterfaces() {
    std::unordered_map<IF_INDEX, Interface> interfaces;

    ULONG size = sizeof(IP_ADAPTER_ADDRESSES) * 100;
    const auto pAddresses = static_cast<PIP_ADAPTER_ADDRESSES>(malloc(size));

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_ALL_INTERFACES, nullptr, pAddresses, &size) == NO_ERROR) {
        for (auto addr = pAddresses; addr != nullptr; addr = addr->Next) {
            if (addr->FriendlyName) {
                interfaces[addr->IfIndex].name = wstring_to_utf8(addr->FriendlyName);
            }
        }
    } else {
        throw std::system_error(errno, std::system_category(), SystemErrorMessage());
    }

    DWORD nSize = 0;
    MIB_IPADDRTABLE* ip_table = nullptr;
    if (GetIpAddrTable(ip_table, &nSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
        ip_table = static_cast<MIB_IPADDRTABLE*>(malloc(nSize));
        if (GetIpAddrTable(ip_table, &nSize, 0) == NO_ERROR) {
            for (int i = 0; i < ip_table->dwNumEntries; ++i) {
                const auto table = ip_table->table[i];

                MIB_IFROW iInfo = {};
                iInfo.dwIndex = table.dwIndex;
                if (GetIfEntry(&iInfo) != NO_ERROR) {
                    continue;
                }

                auto& interface_address = interfaces[iInfo.dwIndex].address;
                interface_address.ips.emplace_back(IpV4{table.dwAddr});

                if (MIB_IF_TYPE_ETHERNET == iInfo.dwType) {
                    Mac mac{};
                    if (mac.data.size() < iInfo.dwPhysAddrLen) {
                        throw std::runtime_error("Insufficient MAC address buffer");
                    }
                    std::memcpy(mac.data.data(), iInfo.bPhysAddr, iInfo.dwPhysAddrLen);
                    interface_address.macs.emplace_back(mac);
                }
            }
        }
        free(ip_table);
    }

    std::vector<Interface> result;
    result.reserve(interfaces.size());
    for (auto& [index, iface] : interfaces) {
        result.emplace_back(std::move(iface));
    }
    return InterfaceVector(std::move(result));
}

PLIFACES_NAMESPACE_END