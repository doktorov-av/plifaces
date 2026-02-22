#include "Plifaces.hpp"
#include <iostream>

void PrintInterface(const plifaces::Interface& iface) {
    auto& addrs = iface.address;
    std::cout << "name: " << iface.name << '\n';
    std::cout << "ips: \n";
    for (const auto& ip : addrs.ips) {
        std::cout << '\t' << ip.ToString() << '\n';
    }

    std::cout << "macs: \n";
    for (const auto& mac : addrs.macs) {
        std::cout << '\t' << mac.ToString() << '\n';
    }
    std::cout << std::endl;
}

void test_get_interface_info() {
    std::string ipV4 = "100.88.141.87";
    const auto interfaces = plifaces::GetInterfaces();

    if (const auto iface = interfaces.FindByIp(plifaces::IpV4::FromStringView(ipV4))) {
        PrintInterface(iface.value());
    }
}

int main() {
    test_get_interface_info();
}