#include "client.hpp"
#include "options.hpp"

int main(int argc, char *argv[]) {
  addpc::options options(argc, argv);
  return addpc::client{options}.run();
}
