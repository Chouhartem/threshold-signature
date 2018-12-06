#ifndef _H_RELICPP
#define _H_RELICPP

class Z;
class G1;
class G2;
class GT;

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
  const g1_t* get() {
    return &t;
  }
  void get_ord(Z &n);
  G1 operator+(G1 a);
  void print();
  bool operator==(G1);
  bool operator!=(G1);
  void rand();
};


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
  bn_t* get()
  {
    return &t;
  }
  void rand_mod(Z);
  void print();
  Z operator+(Z b);
  friend void G1::get_ord(Z&);
};

#endif
