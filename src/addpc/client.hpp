#ifndef ADDPC_CLIENT_H
#define ADDPC_CLIENT_H

#include <addp/action.hpp>

namespace addpc {

class options;

class client {
public:
  explicit client(const options &options) : _options(options) {}

  bool run();
  bool run_action(addp::action &&action);

  // actions
  bool discover();
  bool static_net_config();
  bool reboot();
  bool dhcp_net_config();

private:
  const options &_options;
};

} // namespace addpc

#endif // ADDPC_CLIENT_H
