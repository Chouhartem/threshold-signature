extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "relic++.h"

G1 G1::operator+(G1 a)
{
  g1_t res;
  g1_add(res, *a.get(), t);
  return G1(res);
}

void G1::print()
{
  g1_print(t);
}

bool G1::operator==(G1 a)
{
  return g1_cmp(t, *a.get()) == CMP_EQ;
}

bool G1::operator!=(G1 a)
{
  return g1_cmp(t, *a.get()) == CMP_NE;
}

void G1::get_ord(Z &n)
{
  g1_get_ord(n.t);
}

void G1::rand() {
  g1_rand(t);
}

Z Z::operator+(Z b)
{
  Z res;
  bn_add(*res.get(), t, *b.get());
  return res;
}

void Z::print()
{
  bn_print(t);
}

void Z::rand_mod(Z n)
{
  bn_rand_mod(t, *n.get());
}
