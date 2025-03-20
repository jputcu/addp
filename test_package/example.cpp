#include <addp/packet/packet.hpp>
#include <iostream>

int main() {
  using namespace addp;
  auto p = packet(packet::Type::NONE);
  std::cout << p.type_str() << std::endl;
}
