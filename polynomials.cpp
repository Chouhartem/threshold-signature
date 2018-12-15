extern "C" {
#include <relic.h>
#include <relic_test.h>
#include <relic_bench.h>
}
#include<cmath>
#include "polynomials.h"

std::ostream& operator<<(std::ostream& os, const Polynomial& p)
{
  if (p.coeffs.empty()) {
    os << 0;
    return os;
  }
  unsigned int n = p.coeffs.size();
  for(unsigned int i = n-1; i != 0; --i) {
    if (i == 1) {
      os << p.coeffs[i] << "*X + ";
    }
    else {
      os << p.coeffs[i] << "*X^" << i << " + ";
    }
  }
  os << p.coeffs[0];
  return os;
}

int Polynomial::deg() const
{
  return coeffs.size() - 1;
}

Z& Polynomial::operator[](int i)
{
  return coeffs[i];
}

Z Polynomial::eval(const Z& x)
{
  Z res(0);
  Z eval(1);
  int d = deg();
  for(int i = 0; i <= d; ++i)
  {
    res = res + coeffs[i] * eval;
    eval = x * eval;
  }
  return res;
}

void Polynomial::print()
{
  std::cout << *this << std::endl;
}

Polynomial Polynomial::operator+(const Polynomial& q) const
{
  Polynomial P,Q;
  unsigned int dmax, dmin;
  // deg(P) >= deg(Q)
  if (coeffs.size() > q.coeffs.size()) {
    P = *this;
    Q = q;
    dmax = coeffs.size();
    dmin = q.coeffs.size();
  }
  else {
    P = q;
    Q = *this;
    dmax = q.coeffs.size();
    dmin = coeffs.size();
  }
  Polynomial res(std::vector<Z>(dmax, 0));
  for(unsigned int i = 0; i < dmin ; ++i)
    res.coeffs[i] = P.coeffs[i] + Q.coeffs[i];
  for(unsigned int i = dmin; i < dmax; ++i)
    res.coeffs[i] = P.coeffs[i];
  return res;
}

void Polynomial::operator=(const Polynomial& q)
{
  coeffs.clear();
  std::copy(q.coeffs.begin(), q.coeffs.end(), std::back_inserter(coeffs));
}

bool Polynomial::operator==(const Polynomial& q) const
{
  if (deg() != q.deg())
    return 0;
  for(int i = 0; i <= deg(); ++i) {
    if (coeffs[i] != q.coeffs[i])
      return 0;
  }
  return 1;
}

bool Polynomial::operator!=(const Polynomial& q) const
{
  return !(*this == q);
}

Polynomial operator*(const Z& k, const Polynomial& P)
{
  Polynomial res;
  for(auto x : P.coeffs) {
    res.coeffs.push_back(k*x);
  }
  return res;
}
