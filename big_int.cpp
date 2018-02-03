//
//  big_int.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#include "big_int.hpp"

using namespace std;

big_int::big_int(int x) { bs = x; }

big_int::big_int(bitset<WSIZE> b) { bs = b; }

int big_int::get_kth_bit(int k) const { return bs[k]; }

big_int big_int::operator~() const { return big_int(~bs); }

big_int big_int::operator-() const { return ((~(*this)) + 1); }

bool big_int::operator<(const big_int x) const {
  return bs.to_string() < x.bs.to_string();
}

bool big_int::operator<=(const big_int x) const {
  return bs.to_string() <= x.bs.to_string();
}

bool big_int::operator>(const big_int x) const {
  return bs.to_string() > x.bs.to_string();
}

bool big_int::operator>=(const big_int x) const {
  return bs.to_string() >= x.bs.to_string();
}

bool big_int::operator==(const big_int x) const {
  return bs.to_string() == x.bs.to_string();
}

bool big_int::operator!=(const big_int x) const {
  return bs.to_string() != x.bs.to_string();
}

big_int big_int::operator<<(const int x) const { return big_int(bs << x); }

big_int big_int::operator>>(const int x) const { return big_int(bs >> x); }

big_int big_int::operator|(const big_int x) const { return big_int(bs | x.bs); }

big_int big_int::operator&(const big_int x) const { return big_int(bs & x.bs); }

big_int big_int::operator^(const big_int x) const { return big_int(bs ^ x.bs); }

big_int big_int::operator+(const big_int x) const {
  bitset<WSIZE> carry, res;

  for (int i = 0; i < WSIZE; i++) {
    res[i] = carry[i] ^ bs[i] ^ x.bs[i];

    if ((bs[i] & x.bs[i]) or ((bs[i] | x.bs[i]) & carry[i])) {
      if (i < WSIZE - 1) {
        carry[i + 1] = 1;
      }
    }
  }

  return big_int(res);
}

big_int big_int::operator-(const big_int x) const { return ((*this) + (-x)); }

void big_int::operator+=(big_int x) { (*this) = (*this) + x; }

void big_int::operator-=(big_int x) { (*this) = (*this) - x; }

void big_int::operator|=(big_int x) { (*this) = (*this) | x; }

void big_int::operator&=(big_int x) { (*this) = (*this) & x; }

void big_int::operator^=(big_int x) { (*this) = (*this) ^ x; }

big_int big_int::operator*(const big_int x) const {
  big_int res;

  for (int i = 0; i < WSIZE; i++) {
    if (x.bs[i]) {
      res += (big_int((*this) << i));
    }
  }

  return res;
}

void big_int::operator*=(big_int x) { (*this) = (*this) * x; }

ostream &operator<<(ostream &out, const big_int &bi) {
  for (int i = PSIZE - 1; i >= 0; i--) {
    out << bi.get_kth_bit(i);
  }

  return out;
}

int clz(big_int const &x) {
  int res = 0;

  for (int i = WSIZE - 1; i >= 0; i--) {
    if (x.get_kth_bit(i) == 0) {
      res++;
    }

    else {
      break;
    }
  }

  return res;
}

int most_significant_bit(big_int const &x) { return WSIZE - clz(x) - 1; }
