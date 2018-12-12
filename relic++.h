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
/**
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
  G1(const g1_t a) {
    g1_new(t);
    g1_copy(t, a);
  }
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

  friend G1 operator*(Z k, G1 g);
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
  G2(g2_t a) {
    g2_new(t);
    g2_copy(t, a);
  }
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

  friend G2 operator*(Z k, G2 g);
};


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
  Z(dig_t n)
  {
    bn_null(t);
    bn_new(t);
    bn_set_dig(t, n);
  }
  Z(bn_t n)
  {
    bn_null(t);
    bn_new(t);
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

  friend void G1::get_ord(Z&);
  friend void G2::get_ord(Z&);
  friend G1 operator*(Z k, G1 g);
  friend G2 operator*(Z k, G2 g);
  friend std::ostream& operator<<(std::ostream &ss, Z z);
};

G1 operator*(Z k, G1 g);
G2 operator*(Z k, G2 g);
std::ostream &operator<<(std::ostream &ss, Z z);

#endif
