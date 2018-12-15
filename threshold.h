#ifndef _H_THRESHOLD
#define _H_THRESHOLD
#include <string>
#include <vector>
#include <algorithm>
#include "polynomials.h"
#include "relic++.h"

typedef std::string Msg;

struct Pk {
  G2 g[2];
};

struct Sk {
  Z a1, a2, b1, b2;
  Sk() : a1((dig_t) 0), a2((dig_t) 0), b1((dig_t) 0), b2((dig_t) 0) {}
};

struct Vk {
  G2 v[2];
};

class Player {
  private:
    Sk sk;
    Vk vk;
  public:
};

class Threshold {
  public:
    bool keygen(unsigned int t, unsigned int n);
    std::pair<g1_t, g1_t> share_sign(int i, Msg m);
  private:
    std::vector<Player> p;
};

#endif
