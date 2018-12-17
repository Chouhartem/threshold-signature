extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "threshold.h"

unsigned int Threshold::init_valids()
{
  for(unsigned int i = 0; i < players.size(); ++i)
  {
    if (players[i].is_valid())
      valids.push_back(i);
  }
  return valids.size();
}

bool Threshold::keygen(const unsigned int t, const unsigned int n)
{
  players = std::vector<Player>(n, Player(t, n, *this));
  for(unsigned int i = 0; i < n; ++i) {
    players[i].set_index(i);
  }
  for(unsigned int i = 0; i < n; ++i) {
    players[i].dist_keygen_1();
  }
  /* Check that there is enough valid users */
  if (init_valids() < t) {
    std::cerr << "Error: Not enough valid users" << std::endl;
    return false;
  }
  /* Compute Pk */
  if (pk.g[0].is_infty() && pk.g[1].is_infty()) {
    G2 prod1(players[valids[0]].W1[valids[0]][0]), prod2(players[valids[0]].W2[valids[0]][0]);
    for(unsigned int i = 1; i < valids.size(); ++i) {
      prod1 += players[valids[0]].W1[valids[i]][0];
      prod2 += players[valids[0]].W2[valids[i]][0];
    }
    pk.g[0] = prod1;
    pk.g[1] = prod2;
  }
  else {
    std::cerr << "PK is already initialized" << std::endl;
    return false;
  }
  // Compute VKs and PKs
  for(auto i : valids) {
    if(!players[i].compute_keys()){
      std::cerr << i << ": Error in the computation of VK or SK" << std::endl;
      return false;
    }
  }
  return true;
}

bool Threshold::sign(const Msg& M) const
{
  return true;
}

void Player::dist_keygen_1()
{
  To_Share to_share(t);
  std::vector<G2> w1(t+1);
  std::vector<G2> w2(t+1);
  /* Generate Wi1ℓ and  Wi2ℓ */
  for(unsigned int i = 0; i <= t; ++i) {
    w1[i] = to_share.A1[i] * system.gz + to_share.B1[i] * system.gr;
    w2[i] = to_share.A2[i] * system.gz + to_share.B2[i] * system.gr;
  }
  /* Broadcast */
  for(unsigned int i = 0; i < n; ++i) {
    if (!system.players[i].recv_W(w1, w2, index))
      std::cerr << index << ": Error on broadcast" << std::endl;
  }
  /* Send Share */
  for(unsigned int i = 0; i < n; ++i) {
    Share share(to_share, index + 1);
    if (!system.players[i].recv_share(share, index))
      std::cerr << index << ": Error on player " << i << std::endl;
  }
}

bool Player::compute_keys()
{
  /* Compute user's private key */
  for(auto i : system.valids) {
    sk.a1 += shares[i].a1;
    sk.a2 += shares[i].a2;
    sk.b1 += shares[i].b1;
    sk.b2 += shares[i].b2;
  }

  /* Compute Verification Keys */
  unsigned int v0 = system.valids[0];
  for(auto vi : system.valids)
  {
    vk[vi].v1 = W1[v0][0];
    vk[vi].v2 = W2[v0][0];
    for(auto vj : system.valids) {
      Z pow((dig_t) vi);
      for(unsigned int l = 0; l <= t; ++l)
      {
        if (vj == v0 && l == 0)
          continue;
        vk[vi].v1 += pow * W1[vj][l];
        vk[vi].v2 += pow * W2[vj][l];
        pow *= (dig_t) vi;
      }
    }
  }
  return true;
}

bool Player::recv_W(const std::vector<G2>& w1, const std::vector<G2>& w2, const unsigned int from)
{
  if ((unsigned int)from > W1.size() || !W1[from].empty() || from > W2.size() || !W2[from].empty())
    return false;
  std::copy(w1.begin(), w1.end(), std::back_inserter(W1[from]));
  std::copy(w2.begin(), w2.end(), std::back_inserter(W2[from]));
  return true;
}


bool Player::recv_share(const Share& share, const unsigned int from)
{
  if ((unsigned int)from > shares.size())
    return false;
  shares[from] = share;

  /* Broadcast verification */
  /* W1 */
  G2 eval_bcast(W1[from][0]);
  Z i_pow((dig_t)(from+1));
  for(unsigned int l = 1; l <= t; ++l) {
    eval_bcast = eval_bcast + i_pow * W1[from][l];
    i_pow *= (dig_t)(from + 1);
  }
  if (share.a1 * system.gz + share.b1 * system.gr != eval_bcast)
    return false;

  /* W2 */
  eval_bcast = W2[from][0];
  i_pow = (dig_t)(from+1);
  for(unsigned int l = 1; l <= t; ++l) {
    eval_bcast = eval_bcast + i_pow * W2[from][l];
    i_pow *= (dig_t)(from + 1);
  }
  if (share.a2 * system.gz + share.b2 * system.gr != eval_bcast) {
    system.players[from].disqualify();
    return false;
  }
  return true;
}

void Player::set_index(const int i)
{
  index = i;
}


void Player::disqualify()
{
  disqualified = true;
}

bool Player::is_valid() const
{
  return !disqualified;
}
