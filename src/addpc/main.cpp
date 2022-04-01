#include "client.h"
#include "options.h"

int main(int argc, char *argv[]) {
  addpc::options options(argc, argv);
  addpc::client client(options);

  return client.run();
}
