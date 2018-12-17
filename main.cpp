extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "config.h"
#include "relic++.h"
#include "threshold.h"
#include <iostream>

int main(int argc, char** argv)
{
  if (argc <= 3)
  {
    std::cerr << "Usage : " << argv[0] << " t n M" << std::endl;
    return 1;
  }
  config::relic_init();
	if (pc_param_set_any() == STS_OK) {
    Threshold system;
    unsigned int t = atoi(argv[1]);
    unsigned int n = atoi(argv[2]);
    BENCH_ONCE("Keygen", system.keygen(t,n));
    Msg m(argv[3]);
    std::cout << "Message to be signed: " << m << std::endl;
    BENCH_SMALL("Distributed signature",system.sign(m));
    std::vector<unsigned int> S;
    for(unsigned int i = 0; i <= t; ++i)
      S.push_back(i);
    Sig s;
    BENCH_SMALL("Combination :", s = system.combine(m, S));
    bool res;
    BENCH_SMALL("Verify", res = system.verify(m,s));
    std::cout << "Verify: " << res << std::endl;
  } else
    THROW(ERR_NO_CURVE);
}
