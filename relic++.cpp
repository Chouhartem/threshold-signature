/**
 * @file relic++.cpp
 * @brief Relic wrappen in C++
 * @author Fabrice Mouhartem
 * @date 2018-12-16
 */
extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include "relic++.h"
#define POINT_COMPRESSION 0

/******/
/* G1 */
/******/
G1::G1(const Z& k) : G1() {
  g1_mul_gen(t, k.t);
}
G1 G1::operator+(const G1& b) const
{
  g1_t res;
  g1_add(res, t, b.t);
  return G1(res);
}

void G1::operator+=(const G1& b)
{
  g1_add(t, t, b.t);
}

G1 G1::operator-(const G1& b) const
{
  g1_t res;
  g1_sub(res, t, b.t);
  return G1(res);
}

G1 G1::operator-() const
{
  g1_t res;
  g1_neg(res, t);
  return G1(res);
}

void G1::print() const
{
  g1_print(t);
}

bool G1::operator==(const G1& a) const
{
  return g1_cmp(t, a.t) == CMP_EQ;
}

bool G1::operator!=(const G1& a) const
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

void G1::write(uint8_t* res, const int l, const int compressed) const
{
  g1_write_bin(res, l, t, compressed);
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

G2 G2::operator+(G2 b)
{
  G2 res;
  g2_add(res.t, t, b.t);
  return res;
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

void G2::operator+=(G2 b)
{
  g2_add(t, t, b.t);
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

bool GT::is_unity()
{
  return gt_is_unity(t);
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

GT GT::operator*(GT b)
{
  GT res;
  gt_mul(res.t, t, b.t);
  return res;
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
Z Z::operator+(const Z &b) const
{
  Z res;
  bn_add(res.t, t, b.t);
  return res;
}

Z Z::operator*(const Z &b) const
{
  Z res;
  bn_mul(res.t, t, b.t);
  return res;
}

Z Z::operator*(const unsigned int b) const
{
  Z res;
  bn_mul_dig(res.t, t, b);
  return res;
}

Z Z::operator-(const Z &b) const
{
  Z res;
  bn_sub(res.t, t, b.t);
  return res;
}
Z Z::operator/(const Z &b) const
{
  Z res;
  bn_div(res.t, t, b.t);
  // -1/1 = -2
  if (bn_sign(res.t) == BN_NEG)
    bn_add_dig(res.t, res.t, 1);
  return res;
}

Z Z::operator%(const Z& b) const
{
  Z res;
  bn_mod(res.t, t, b.t);
  return res;
}

unsigned int Z::size_bin() const
{
  return bn_size_bin(t);
}

void Z::print() const
{
  bn_print(t);
}

bool Z::is_zero() const
{
  return bn_is_zero(t);
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

Z Z::operator-() const
{
  Z res;
  bn_neg(res.t, t);
  return res;
}

Z Z::operator+() const
{
  Z res(t);
  return res;
}

bool Z::operator==(const Z& b) const
{
  return bn_cmp(t, b.t) == CMP_EQ;
}

bool Z::operator!=(const Z& b) const
{
  return bn_cmp(t, b.t) != CMP_EQ;
}

bool Z::operator<(const Z& b) const
{
  return bn_cmp(t, b.t) == CMP_LT;
}

bool Z::operator>(const Z& b) const
{
  return bn_cmp(t, b.t) == CMP_GT;
}

bool Z::operator>=(const Z& b) const
{
  int cmp = bn_cmp(t, b.t);
  return cmp == CMP_GT || cmp == CMP_EQ;
}
bool Z::operator<=(const Z& b) const
{
  int cmp = bn_cmp(t, b.t);
  return cmp == CMP_LT || cmp == CMP_EQ;
}

void Z::operator=(const Z& b)
{
  bn_copy(t, b.t);
}

void Z::operator*=(const Z& b)
{
  bn_mul(t, t, b.t);
}

void Z::operator+=(const Z& b)
{
  bn_add(t, t, b.t);
}

Z Z::inv_mod_p(const Z& p) const
{
  Z a;
  bn_t b, d;
  bn_null(b); bn_null(d);
  bn_new(b); bn_new(d);
  bn_gcd_ext(d, a.t, b, t, p.t);
  bn_free(b); bn_free(d);
  bn_mod(a.t, a.t, p.t);
  return a;
}


/******************/
/* Hash functions */
/******************/
void H::compute(uint8_t* buf, int l)
{
  uint8_t salt_buf[l + salt_size];
  for(unsigned int i = 0; i < salt_size; ++i)
    salt_buf[i] = salt[i];
  for(unsigned int i = salt_size; i < l + salt_size; ++i)
    salt_buf[i] = buf[i];
  md_map_sh512(h, salt_buf, l);
}

int H::set_salt(uint8_t* s, unsigned int l)
{
  unsigned int bound = (l<salt_size)?l:salt_size;
  for(unsigned int i = 0; i < bound; ++i)
    salt[i] = s[i];
  if (l > salt_size)
    return 1;
  return 0;
}

int H::set_salt(const Z& n)
{
  if (n.size_bin() > salt_size)
    return 1;
  bn_write_bin(salt, salt_size, n.t);
  return 0;
}

Z H::rand_salt()
{
  bn_t n;
  bn_rand(n, BN_POS, 8 * salt_size);
  bn_write_bin(salt, salt_size, n);
  return n;
}

void H::print()
{
  for(unsigned int i = 0; i < size; ++i) {
    printf("%.2x", h[i]);
  }
  printf("\n");
}

void H::from(const Z& n)
{
  int l = bn_size_bin(n.t);
  uint8_t buf[l];
  bn_write_bin(buf, l, n.t);
  compute(buf, l);
}
void H::from(const G1& g)
{
  int l = g1_size_bin(g.t, POINT_COMPRESSION);
  uint8_t buf[l];
  g1_write_bin(buf, l, g.t, POINT_COMPRESSION);
  compute(buf, l);
}

void H::from(G2& g)
{
  int l = g2_size_bin(g.t, POINT_COMPRESSION);
  uint8_t buf[l];
  g2_write_bin(buf, l, g.t, POINT_COMPRESSION);
  compute(buf, l);
}

void H::from(GT& g)
{
  int l = gt_size_bin(g.t, POINT_COMPRESSION);
  uint8_t buf[l];
  gt_write_bin(buf, l, g.t, POINT_COMPRESSION);
  compute(buf, l);
}

void H::from(const std::string& s)
{
  const unsigned int l = s.size();
  uint8_t buf[l];
  const char* m = s.c_str();
  for(unsigned int i = 0; i < l; ++i)
    buf[i] = m[i];
  compute(buf, l);
}

void H::to(Z& n) const
{
  bn_read_bin(n.t, h, size);
}

void H::to(G1& g) const
{
  g1_map(g.t, h, size);
}

void H::to(G2& g)
{
  g2_map(g.t, h, size);
}

