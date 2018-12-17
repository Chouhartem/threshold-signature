#ifndef _H_THRESHOLD
#define _H_THRESHOLD
#include <string>
#include <vector>
#include <algorithm>
#include "polynomials.h"
#include "relic++.h"

typedef std::string Msg;

class Player;
class Threshold;

struct Pk {
  G2 g[2];
};

struct Sk {
  Z a1, a2, b1, b2;
  Sk() : a1((dig_t) 0), a2((dig_t) 0), b1((dig_t) 0), b2((dig_t) 0) {}
};

struct Vk {
  G2 v1, v2;
  Vk()
  {
    v1.gen();
    v2.gen();
  }
};

struct To_Share {
  Polynomial A1, A2, B1, B2;
  To_Share() : A1(), A2(), B1(), B2() {}
  To_Share(const unsigned int d) : A1(d), A2(d), B1(d), B2(d) {}
};

struct Share {
  Z a1, a2, b1, b2;
  Share() {}
  Share(const To_Share& S, const unsigned int& i)
  {
    a1 = S.A1.eval_p(Z((dig_t) i));
    a2 = S.A2.eval_p(Z((dig_t) i));
    b1 = S.B1.eval_p(Z((dig_t) i));
    b2 = S.B2.eval_p(Z((dig_t) i));
  }
};


class Player {
  bool disqualified;
  unsigned int index;
  Sk sk;
  std::vector<Vk> vk;
  std::vector<Share> shares;
  public:
  std::vector<std::vector<G2>> W1;
  std::vector<std::vector<G2>> W2;
  Threshold& system;
  unsigned int n;
  unsigned int t;
  Player(const unsigned int tt, const unsigned int nn, Threshold& sys) :disqualified(false), index(0), system(sys)
  {
    t = tt;
    n = nn;
    vk = std::vector<Vk>(n);
    shares = std::vector<Share>(n);
    W1 = std::vector<std::vector<G2>>(n);
    W2 = std::vector<std::vector<G2>>(n);
  }
  void disqualify();
  bool is_valid() const;
  void set_index(const int i);
  void dist_keygen_1();
  bool recv_W(const std::vector<G2>& w1, const std::vector<G2>& w2, const unsigned int from);
  bool recv_share(const Share& share, const unsigned int from);
  bool compute_keys();
};

class Threshold {
  public:
  Pk pk;
  G2 gz, gr;
  Threshold()
  {
    pk.g[0].set_infty();
    pk.g[1].set_infty();
    gz.rand();
    gr.rand();
  }
  std::vector<Player> players;
  std::vector<unsigned int> valids;

  unsigned int init_valids();

  bool keygen(const unsigned int t, const unsigned int n);
  std::pair<g1_t, g1_t> share_sign(int i, Msg m);
  bool sign(const Msg& M) const;
};

#endif
