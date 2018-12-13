extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "config.h"
#include <iostream>

namespace config {
  void relic_init()
  {
    if (core_init() != STS_OK) {
      core_clean();
      std::cerr << "Error at init" << std::endl;
    }
    //conf_print();
  }
}
