#include "dhcp_net_config.hpp"
#include <iostream>
using namespace addp;

void dhcp_net_config::print_brief(const boost::asio::ip::udp::endpoint & /*sender*/,
                                  const packet &response) const {
  for (const field &f : response.fields())
    if (f.type() == field::FT_RESULT_MSG) {
      std::cout << f.value_str() << "\n";
      break;
    }
}
