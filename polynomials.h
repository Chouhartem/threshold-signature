#ifndef _H_POLYNOMIALS
#define _H_POLYNOMIALS
#include <vector>
#include <ostream>
#include <algorithm>
#include "relic++.h"

class Polynomial;

class Polynomial
{
  std::vector<Z> coeffs;
  public:
  Polynomial() {}
  Polynomial(unsigned int n)
  {
    G1 g;
    g.gen();
    Z p, a;
    g.get_ord(p);
    for(unsigned int i = 0; i <= n; ++i) {
      a.rand_mod(p);
      coeffs.push_back(a);
    }
  }
  Polynomial(const std::vector<Z>& v)
  {
    coeffs.clear();
    std::copy(v.begin(), v.end(), std::back_inserter(coeffs));
  }
  int deg() const;
  Z eval(const Z& x);
  void print();
  Polynomial operator+(const Polynomial& q) const;
  Z& operator[](int i);
  void operator=(const Polynomial& q);
  bool operator==(const Polynomial& q) const;
  bool operator!=(const Polynomial& q) const;
  friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);
  friend Polynomial operator*(const Z& k, const Polynomial& P);
};

std::ostream& operator<<(std::ostream& os, const Polynomial& p);
Polynomial operator*(const Z& k, const Polynomial& P);
Z lagrange_zero(const std::vector<std::pair<Z, Z>>& parts);

#endif
