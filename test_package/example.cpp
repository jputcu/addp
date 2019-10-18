#include <iostream>
#include <addp/packet/packet.h>

int main() {
    using namespace addp;
    auto p = packet(packet::PT_NONE);
    std::cout << p.type_str() << std::endl;
}
