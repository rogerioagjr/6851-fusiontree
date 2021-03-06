//
//  big_int.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#ifndef big_int_hpp
#define big_int_hpp

#ifndef NAIVE

#include <stdio.h>
#include <immintrin.h>

#include <bitset>
#include "constants.hpp"

using namespace std;

class big_int {
 private:
  uint64_t num[BIGARRAYSIZE];

  __m256i load_to_m256i(int index) const;

 public:
  big_int(int x = 0);

  big_int(const bitset<WSIZE> &b);

  int to_int() const;

  int get_kth_bit(int k) const;

  void test_extraction() const;

  big_int operator~() const;

  big_int operator-() const;

  bool operator<(const big_int x) const;

  bool operator<=(const big_int x) const;

  bool operator>(const big_int x) const;

  bool operator>=(const big_int x) const;

  bool operator==(const big_int x) const;

  bool operator!=(const big_int x) const;

  big_int operator<<(const int x) const;

  big_int operator>>(const int x) const;

  big_int operator|(const big_int x) const;

  big_int operator&(const big_int x) const;

  big_int operator^(const big_int x) const;

  big_int operator+(const big_int x) const;

  big_int operator-(const big_int x) const;

  void operator+=(big_int x);

  void operator-=(big_int x);

  void operator|=(big_int x);

  void operator&=(big_int x);

  void operator^=(big_int x);

  big_int operator*(const big_int x) const;

  void operator*=(big_int x);

  bitset<WSIZE> to_bitset() const;
};

ostream &operator<<(ostream &out, const big_int &bi);

int clz(big_int const &x);

int most_significant_bit(big_int const &x);

#else

#include <stdio.h>

#include <bitset>
#include "constants.hpp"

using namespace std;

class big_int {
 private:
  bitset<WSIZE> bs;

 public:
  big_int(int x = 0);

  big_int(const bitset<WSIZE> &b);

  int to_int() const;

  int get_kth_bit(int k) const;

  void test_extraction() const;

  big_int operator~() const;

  big_int operator-() const;

  bool operator<(const big_int x) const;

  bool operator<=(const big_int x) const;

  bool operator>(const big_int x) const;

  bool operator>=(const big_int x) const;

  bool operator==(const big_int x) const;

  bool operator!=(const big_int x) const;

  big_int operator<<(const int x) const;

  big_int operator>>(const int x) const;

  big_int operator|(const big_int x) const;

  big_int operator&(const big_int x) const;

  big_int operator^(const big_int x) const;

  big_int operator+(const big_int x) const;

  big_int operator-(const big_int x) const;

  void operator+=(big_int x);

  void operator-=(big_int x);

  void operator|=(big_int x);

  void operator&=(big_int x);

  void operator^=(big_int x);

  big_int operator*(const big_int x) const;

  void operator*=(big_int x);
};

ostream &operator<<(ostream &out, const big_int &bi);

int clz(big_int const &x);

int most_significant_bit(big_int const &x);

#endif

#endif /* big_int_hpp */
