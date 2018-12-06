#ifndef _H_THRESHOLD
#define _H_THRESHOLD
#include <string>
#include <vector>
#include <algorithm>

typedef std::string Msg;

struct Sk {
  bn_t a1, a2, b1, b2;
};

class Threshold {
  public:
    bool keygen(unsigned int t, unsigned int n);
    std::pair<g1_t, g1_t> share_sign(int i, Msg m);
  private:
    std::vector<Sk> sk;
};

#endif
