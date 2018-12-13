extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "relic++.h"

/******/
/* G1 */
/******/
G1::G1(const Z& k) : G1() {
  g1_mul_gen(t, k.t);
}
G1 G1::operator+(G1 b)
{
  g1_t res;
  g1_add(res, t, b.t);
  return G1(res);
}

G1 G1::operator-(G1 b)
{
  g1_t res;
  g1_sub(res, t, b.t);
  return G1(res);
}

G1 G1::operator-()
{
  g1_t res;
  g1_neg(res, t);
  return G1(res);
}

void G1::print()
{
  g1_print(t);
}

bool G1::operator==(G1 a)
{
  return g1_cmp(t, a.t) == CMP_EQ;
}

bool G1::operator!=(G1 a)
{
  return g1_cmp(t, a.t) == CMP_NE;
}

void G1::get_ord(Z &n)
{
  g1_get_ord(n.t);
}

Z G1::get_ord()
{
  bn_t z;
  g1_get_ord(z);
  return Z(z);
}

void G1::gen()
{
  g1_get_gen(t);
}

void G1::set_infty()
{
  g1_set_infty(t);
}

bool G1::valid()
{
  return g1_is_valid(t);
}

bool G1::is_infty()
{
  return g1_is_infty(t);
}

void G1::rand() {
  g1_rand(t);
}

G1 operator*(Z k, G1 g)
{
  G1 res;
  g1_mul(res.t, g.t, k.t);
  return res;
}


/******/
/* G2 */
/******/
G2::G2(const Z& k) : G2() {
  bn_t kt;
  bn_copy(kt, k.t);
  g2_mul_gen(t, kt);
}

void G2::get_ord(Z &n)
{
  g2_get_ord(n.t);
}

Z G2::get_ord()
{
  bn_t z;
  g2_get_ord(z);
  return Z(z);
}

void G2::gen()
{
  g2_get_gen(t);
}

void G2::set_infty()
{
  g2_set_infty(t);
}

bool G2::is_infty()
{
  return g2_is_infty(t);
}

bool G2::valid()
{
  return g2_is_valid(t);
}

G2 G2::operator-(G2 b)
{
  g2_t res;
  g2_sub(res, t, b.t);
  return G2(res);
}

G2 G2::operator-()
{
  g2_t res;
  g2_neg(res, t);
  return G2(res);
}

bool G2::operator==(G2 a)
{
  return g2_cmp(t, a.t) == CMP_EQ;
}

bool G2::operator!=(G2 a)
{
  return g2_cmp(t, a.t) == CMP_NE;
}

G2 operator*(Z k, G2 g)
{
  G2 res;
  g2_mul(res.t, g.t, k.t);
  return res;
}

void G2::rand()
{
  g2_rand(t);
}

void G2::print()
{
  g2_print(t);
}

/******/
/* GT */
/******/
GT::GT(const Z& k) : GT()
{
  bn_t kt;
  bn_copy(kt, k.t);
  gt_get_gen(t);
  gt_exp(t, t, kt);
}

void GT::print()
{
  gt_print(t);
}

void GT::gen()
{
  gt_get_gen(t);
}

bool GT::operator==(GT& a)
{
  return gt_cmp(t, a.t) == CMP_EQ;
}

bool GT::operator!=(GT& a)
{
  return gt_cmp(t, a.t) == CMP_NE;
}

GT pairing(G1 a, G2 b)
{
  gt_t res;
  pc_map(res, a.t, b.t);
  return GT(res);
}

void GT::operator=(GT a)
{
  gt_copy(t, a.t);
}

GT GT::operator^(const Z& k)
{
  bn_t kt;
  bn_copy(kt, k.t);
  gt_t res;
  gt_exp(res, t, kt);
  return GT(res);
}

/*****/
/* Z */
/*****/
Z Z::operator+(Z &b)
{
  Z res;
  bn_add(res.t, t, b.t);
  return res;
}

Z Z::operator*(Z &b)
{
  Z res;
  bn_mul(res.t, t, b.t);
  return res;
}
Z Z::operator-(Z &b)
{
  Z res;
  bn_sub(res.t, t, b.t);
  return res;
}
Z Z::operator/(Z &b)
{
  Z res;
  bn_div(res.t, t, b.t);
  return res;
}

void Z::print()
{
  bn_print(t);
}

void Z::rand_mod(Z n)
{
  bn_rand_mod(t, n.t);
}

std::ostream& operator<<(std::ostream &ss, Z z)
{
  char n = bn_size_str(z.t, 10);
  char s[n+1];
  bn_write_str(s, n, z.t, 10);
  ss << s;
  return ss;
}

bool Z::operator==(Z& b)
{
  return bn_cmp(t, b.t) == CMP_EQ;
}

bool Z::operator<(Z& b)
{
  return bn_cmp(t, b.t) == CMP_LT;
}

bool Z::operator>(Z& b)
{
  return bn_cmp(t, b.t) == CMP_GT;
}

bool Z::operator>=(Z& b)
{
  int cmp = bn_cmp(t, b.t);
  return cmp == CMP_GT || cmp == CMP_EQ;
}
bool Z::operator<=(Z& b)
{
  int cmp = bn_cmp(t, b.t);
  return cmp == CMP_LT || cmp == CMP_EQ;
}

void Z::operator=(Z& b)
{
  bn_copy(t, b.t);
}
