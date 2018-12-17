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
  sigma = std::vector<Sig>(n);
  vk = std::vector<Vk>(n);
  W1 = std::vector<std::vector<G2>>(n);
  W2 = std::vector<std::vector<G2>>(n);
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
    G2 prod1, prod2;
    for(unsigned int i = 0; i < valids.size(); ++i) {
      prod1 += W1[valids[i]][0];
      prod2 += W2[valids[i]][0];
    }
    pk.g[0] = prod1;
    pk.g[1] = prod2;
  }
  else {
    std::cerr << "PK is already initialized" << std::endl;
    return false;
  }

  /* Compute SKs */
  for(auto i : valids)
    players[i].compute_sk();
  return true;

  /* Compute VKs */
  for(auto i : valids)
    compute_vk(i, t);
}

bool Threshold::sign(const Msg& M)
{
  /* Generate signatures */
  for(auto i : valids) {
    if (!players[i].sign(M)) {
      std::cerr << i << ": error while generating signature for message " << M << std::endl;
      return false;
    }
  }
  return true;
}

Sig Threshold::combine(const Msg& M, std::vector<unsigned int>& S)
{
  Sig s;
  if (S.size() <= players[0].t) {
    std::cerr << "Not enough users to perform Lagrange reconstruction" << std::endl;
    return s;
  }

  /* Verify signature shares */
  for(auto i : S)
  {
    if(!players[S[0]].verify(M, i))
    {
      std::cerr << "Combine: Invalid signature from user " << i << std::endl;
    }
  }

  /* Lagrange reconstruction */
  G1 g;
  Z p;
  g.get_ord(p);
  for(auto i : S) {
    Z part_num(1);
    Z part_den(1);
    for(auto j : S) {
      if (i == j)
        continue;
      part_num = (part_num * (j+1)) % p;
      part_num = p - part_num;
      part_den = (part_den * (Z((dig_t) i)-Z((dig_t) j))) % p;
    }
    Z coeff = (part_num * part_den.inv_mod_p(p)) % p;
    s.z = s.z + coeff * sigma[i].z;
    s.r = s.r + coeff * sigma[i].r;
  }
  return s;
}

bool Threshold::verify(const Msg& M, const Sig& s)
{
  /* Compute hashes of the message */
  G1 h1, h2;
  H hash;

  hash.set_salt(s1);
  hash.from(M);
  hash.to(h1);

  hash.set_salt(s2);
  hash.from(M);
  hash.to(h2);
  /* Verify */
  return (
      pairing(s.z, gz) *
      pairing(s.r, gr) *
      pairing(h1, pk.g[0]) *
      pairing(h2, pk.g[1])
      ).is_unity();
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
  if (!system.recv_W(w1, w2, index))
    std::cerr << index << ": Error on broadcast" << std::endl;

  /* Send Share */
  for(unsigned int j = 0; j < n; ++j) {
    Share share(to_share, j+1);
    if (!system.players[j].recv_share(share, index))
      std::cerr << index << ": Error on share exchange to " << j << std::endl;
  }
}

bool Player::compute_sk()
{
  /* Compute user's private key */
  for(auto i : system.valids) {
    sk.a1 += shares[i].a1;
    sk.a2 += shares[i].a2;
    sk.b1 += shares[i].b1;
    sk.b2 += shares[i].b2;
  }
  return true;
}

bool Threshold::compute_vk(const unsigned int i, const unsigned int t)
{
  /* Compute Verification Keys */
  vk[i].v1.set_infty();
  vk[i].v2.set_infty();
  for(auto vj : valids) {
    Z pow((dig_t) 1);
    for(unsigned int l = 0; l <= t; ++l)
    {
      vk[i].v1 += pow * W1[vj][l];
      vk[i].v2 += pow * W2[vj][l];
      pow *= (dig_t) (i+1);
    }
  }
  return true;
}

bool Threshold::recv_W(const std::vector<G2>& w1, const std::vector<G2>& w2, const unsigned int from)
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
  G2 eval_bcast;
  eval_bcast.set_infty();
  Z i_pow((dig_t) 1);
  for(unsigned int l = 0; l <= t; ++l) {
    eval_bcast = eval_bcast + i_pow * system.W1[from][l];
    i_pow *= (dig_t)(index+1);
  }
  if (share.a1 * system.gz + share.b1 * system.gr != eval_bcast)
    return false;

  /* W2 */
  eval_bcast.set_infty();
  i_pow = (dig_t) 1;
  for(unsigned int l = 0; l <= t; ++l) {
    eval_bcast = eval_bcast + i_pow * system.W2[from][l];
    i_pow *= (dig_t)(index+1);
  }
  if (share.a2 * system.gz + share.b2 * system.gr != eval_bcast) {
    system.players[from].disqualify();
    return false;
  }
  return true;
}

bool Player::sign(const Msg& M) const
{
  /* Compute hashes of the message */
  G1 h1, h2;
  H hash;

  hash.set_salt(system.s1);
  hash.from(M);
  hash.to(h1);

  hash.set_salt(system.s2);
  hash.from(M);
  hash.to(h2);

  /* Generate signature share */
  system.sigma[index].z = -sk.a1 * h1 - sk.a2 * h2;
  system.sigma[index].r = -sk.b1 * h1 - sk.b2 * h2;
  return true;
}

bool Player::verify(const Msg& M, const unsigned int from)
{
  /* Compute hashes of the message */
  G1 h1, h2;
  H hash;

  hash.set_salt(system.s1);
  hash.from(M);
  hash.to(h1);

  hash.set_salt(system.s2);
  hash.from(M);
  hash.to(h2);

  /* Verify */
  G1 r,z;
  r = system.sigma[from].r;
  z = system.sigma[from].z;
  return (pairing(z, system.gz) *
      pairing(r, system.gr) *
      pairing(h1, system.vk[from].v1) *
      pairing(h2, system.vk[from].v2)).is_unity();
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
