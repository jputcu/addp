#ifndef ADDPC_OPTIONS_H
#define ADDPC_OPTIONS_H

#include <addp/addp.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace addpc {

class options : public addp::options {
public:
  options(int argc, char *argv[]);

  std::string listen() const;
  size_t timeout() const;
  size_t max_count() const;

  std::string action() const;
  std::string mac() const;
  std::vector<std::string> args() const;

  // action options
  std::string password() const;
  std::string ip() const;
  std::string subnet() const;
  std::string gateway() const;
  bool dhcp() const;

protected:
  void opt_parse(int argc, char *argv[]);

  boost::program_options::options_description all_options() const override;
  boost::program_options::positional_options_description positional_options() const override;
  boost::program_options::options_description visible_options() const override;

private:
  boost::program_options::options_description addpc_options() const;
  boost::program_options::options_description addpc_hidden_options() const;

  size_t _password_index;
};

} // namespace addpc

#endif // ADDPC_OPTIONS_H
