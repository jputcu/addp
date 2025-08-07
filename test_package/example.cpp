#include <addp/packet.hpp>
#include <iostream>

int main() {
  using namespace addp;
  std::cout << packet_type::DISCOVERY_RESPONSE<< "\n";
  return EXIT_SUCCESS;
}
