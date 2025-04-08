#include <addp/packet.hpp>
#include <iostream>

int main() {
  using namespace addp;
  std::cout << packet_type::REBOOT_REQUEST << "\n";
  return EXIT_SUCCESS;
}
