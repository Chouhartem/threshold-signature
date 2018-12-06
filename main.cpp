extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "config.h"
#include "relic++.h"
#include "threshold.h"
#include <iostream>

int main()
{
  config::relic_init();
	if (pc_param_set_any() == STS_OK) {
    // TBA
  } else
    THROW(ERR_NO_CURVE);
}
