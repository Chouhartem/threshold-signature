/* Copyright (C) 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */
#ifndef _H_RELICPP
#define _H_RELICPP
#include<ostream>
#include<iostream>
/*
 * @file relic++.h
 * @brief Relic wrapper in c++
 * @author Fabrice Mouhartem
 * @version 0.1
 * @date 2018-12-10
 */


class Z;
class G1;
class G2;
class GT;
class H;

/**
 * @brief Group elements in G1
 */
class G1 {
  g1_t t;
  public:
  G1() {
    g1_null(t);
    g1_new(t);
  }
  G1(const g1_t a) : G1() {
    g1_copy(t, a);
  }
  G1(const G1& a) : G1() {
    g1_copy(t, a.t);
  }
  G1(const Z& k);
  ~G1() {
    g1_free(t);
  }
  void get_ord(Z &n);
  Z get_ord();
  void gen();
  void set_infty();
  bool is_infty();
  bool valid();
  G1 operator+(G1 b);
  G1 operator-(G1 b);
  G1 operator-();
  void print();
  bool operator==(G1);
  bool operator!=(G1);
  void rand();
  void write(uint8_t* res, const int l, const int compressed);

  friend G1 operator*(Z k, G1 g);
  friend GT pairing(G1 a, G2 b);
  friend class H;
};

/**
 * @brief Group elements in G2
 */
class G2 {
  g2_t t;
  public:
  G2() {
    g2_null(t);
    g2_new(t);
  }
  G2(g2_t a) : G2() {
    g2_copy(t, a);
  }
  G2(const Z& k);
  ~G2() {
    g2_free(t);
  }
  void get_ord(Z &n);
  Z get_ord();
  void gen();
  void set_infty();
  bool is_infty();
  bool valid();
  G2 operator+(G2 b);
  G2 operator-(G2 b);
  G2 operator-();
  void print();
  bool operator==(G2);
  bool operator!=(G2);
  void rand();

  friend H;
  friend G2 operator*(Z k, G2 g);
  friend GT pairing(G1 a, G2 b);
};

/**
 * @brief Target group elements
 */
class GT {
  gt_t t;
  public:
  GT() {
    gt_null(t);
    gt_new(t);
  }
  GT(gt_t a) {
    gt_new(t);
    gt_copy(t, a);
  }
  GT(const Z& k);
  ~GT() {
    gt_free(t);
  }
  void print();
  GT operator*(GT b);
  void gen();
  void operator=(GT a);
  bool operator==(GT& a);
  bool operator!=(GT& a);
  GT operator^(const Z& a);

  friend H;
};

GT pairing(G1 a, G2 b);


/**
 * @brief Scalar elements
 */
class Z {
  bn_t t;
  public:
  Z() {
    bn_null(t);
    bn_new(t);
  }
  Z(int n) : Z()
  {
    bn_set_dig(t, (dis_t) n);
  }
  Z(dig_t n) : Z()
  {
    bn_set_dig(t, n);
  }
  Z(bn_t n) : Z()
  {
    bn_copy(t,n);
  }
  ~Z() {
    bn_clean(t);
    bn_free(t);
  }
  void rand_mod(Z);
  void print();
  Z operator+(Z &b);
  Z operator*(Z &b);
  Z operator/(Z &b);
  Z operator-(Z &b);
  bool operator==(Z& b);
  bool operator<(Z& b);
  bool operator>(Z& b);
  bool operator>=(Z& b);
  bool operator<=(Z& b);
  void operator=(Z& b);

  friend class G1;
  friend class G2;
  friend class GT;
  friend H;
  friend G1 operator*(Z k, G1 g);
  friend G2 operator*(Z k, G2 g);
  friend std::ostream& operator<<(std::ostream &ss, Z z);
};

/**
 * @brief Hash functions
 */
class H{
  const unsigned int size = 64;
  const unsigned int salt_size = 32;
  uint8_t h[64];
  uint8_t salt[32];
  public:
  H() {
    for(unsigned int i = 0; i < size; ++i)
      h[i] = 0;
    for(unsigned int i = 0; i < salt_size; ++i)
      salt[i] = 0;
  }
  int set_salt(uint8_t* s, unsigned int l);
  void rand_salt();
  void print();
  void compute(uint8_t* buf, int l);
  void from(const Z& n);
  void from(const G1& g);
  void from(G2& g);
  void from(GT& g);
  void to(Z& n);
  void to(G1& g);
  void to(G2& g);
};

G1 operator*(Z k, G1 g);
G2 operator*(Z k, G2 g);
std::ostream &operator<<(std::ostream &ss, Z z);

#endif
