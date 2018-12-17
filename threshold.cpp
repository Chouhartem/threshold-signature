extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "threshold.h"

bool Threshold::keygen(const unsigned int t, const unsigned int n)
{
  players = std::vector<Player>(n, Player(t, n, *this));
  for(unsigned int i = 0; i < n; ++i) {
    players[i].set_index(i);
  }
  for(unsigned int i = 0; i < n; ++i) {
    players[i].dist_keygen_1();
  }
  return true;
}

void Player::dist_keygen_1()
{
  To_Share to_share(t);
  std::vector<G2> W1(t+1);
  std::vector<G2> W2(t+1);
  /* Generate Wi1ℓ and  Wi2ℓ */
  for(unsigned int i = 0; i <= t; ++i) {
    W1[i] = to_share.A1[i] * system.gz + to_share.B1[i] * system.gr;
    W2[i] = to_share.A2[i] * system.gz + to_share.B2[i] * system.gr;
  }
  /* Broadcast */
  for(unsigned int i = 0; i < n; ++i) {
    if (i == index)
      continue;
    if (!system.players[i].recv_W(W1, W2, index))
      std::cerr << index << ": Error on broadcast" << std::endl;
  }
  /* Send Share */
  for(unsigned int i = 0; i < n; ++i) {
    if (i == index)
      continue;
    Share share(to_share, index + 1);
    if (!system.players[i].recv_share(share, index))
      std::cerr << index << ": Error on player " << i << std::endl;
  }
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
  if (share.a2 * system.gz + share.b2 * system.gr != eval_bcast)
    return false;

  return true;
}

void Player::set_index(const int i)
{
  index = i;
}
