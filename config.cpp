extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "config.h"

namespace config {
  void relic_init()
  {
    if (core_init() != STS_OK) {
      core_clean();
    }
    //conf_print();
  }
}
