//
//  big_int.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#ifndef NAIVE

#include "big_int.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

const uint64_t MAXI_64 = 0xFFFFFFFFFFFFFFFF;

/** HELPER FUNCTIONS **/

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

int get_kth_bit_from_uint(uint64_t val, int k) {
  return ((val & (((uint64_t)1) << k)) == 0) ? 0 : 1;
}

void print_in_binary(uint64_t val) {
  for (int i = 63; i >= 0; i--) {
    cout << get_kth_bit_from_uint(val, i);
  }
}

void print_m256i(__m256i val) {
  for (int i = 0; i < 4; i++) {
    print_in_binary(_mm256_extract_epi64(val, i));
  }
}

void store_from_m256i(uint64_t *ptr, __m256i &val) {
  for (int k = 0; k < 4; k++) {
    *(ptr + k) = _mm256_extract_epi64(val, k);
  }
}

/** LIBRARY **/

big_int::big_int(int x) {
  // cout << "Called big_int(int " << x << ")" << endl;
  num[0] = x;
  for (int i = 1; i < BIGARRAYSIZE; i++) {
    num[i] = 0;
  }
}

big_int::big_int(const bitset<WSIZE> &b) {
  for (int i = 0; i < BIGARRAYSIZE; i++) {
    uint64_t cur_value = 0;
    int cur_start = i * 64;
    for (int k = cur_start; k < cur_start + 64 && k < WSIZE; k++) {
      cur_value =
          cur_value + (((uint64_t)((b[k] == 0) ? 0 : 1)) << (k - cur_start));
    }
    num[i] = cur_value;
    cur_start += 64;
  }
}

__m256i big_int::load_to_m256i(int index) const {
  // cout << "Called load_to_m256i(" << index << ")" << endl;
  return _mm256_setr_epi64x(num[index], num[index + 1], num[index + 2],
                            num[index + 3]);
}

void big_int::test_extraction() const {
  cout << "test_extraction(" << *this << ")" << endl;
  for (int i = 0; i < BIGARRAYSIZE; i += 4) {
    __m256i this_vec = this->load_to_m256i(i);
    for (int k = 0; k < 4; k++) {
      uint64_t cur_val = _mm256_extract_epi64(this_vec, k);
      print_in_binary(cur_val);
      cout << endl;
    }
  }
}

int big_int::get_kth_bit(int k) const {
  int array_idx = k / 64;
  int element_idx = k % 64;
  return get_kth_bit_from_uint(num[array_idx], element_idx);
}

int big_int::to_int() const {
  // cout << "Called to_int()" << endl;
  return num[0];
}

big_int big_int::operator~() const {
  // cout << "Called ~" << endl;
  big_int not_val = big_int();
  __m256i all_ones = _mm256_setr_epi64x(MAXI_64, MAXI_64, MAXI_64, MAXI_64);
  for (int i = 0; i < BIGARRAYSIZE; i += 4) {
    __m256i result = _mm256_andnot_si256(this->load_to_m256i(i), all_ones);
    store_from_m256i(&not_val.num[i], result);
  }
  return not_val;
}

int get_msb_from_uint(uint64_t &x) {
  for (int i = 63; i >= 0; i--) {
    if ((x & (((uint64_t)1) << i)) != 0) {
      return i;
    }
  }
  return -1;
}

bool big_int::operator>(const big_int x) const {
  for (int i = BIGARRAYSIZE - 4; i >= 0; i -= 4) {
    __m256i this_vec = this->load_to_m256i(i);
    __m256i that_vec = x.load_to_m256i(i);
    __m256i xor_vec = _mm256_xor_si256(this_vec, that_vec);
    for (int k = 3; k >= 0; k--) {
      uint64_t cur_val = _mm256_extract_epi64(xor_vec, k);
      if (cur_val != 0) {
        int idx = get_msb_from_uint(cur_val);
        if (get_kth_bit_from_uint(this->num[i + k], idx) == 1) {
          return true;
        } else {
          return false;
        }
      }
    }
  }
  return false;
}

bool big_int::operator<(const big_int x) const {
  // cout << "Called < between " << *this << " and " << x << endl;
  return (x > *this);
}

bool big_int::operator<=(const big_int x) const { return !(*this > x); }

bool big_int::operator>=(const big_int x) const { return !(*this < x); }

bool big_int::operator==(const big_int x) const {
  // cout << "Called == for " << (*this) << " and " << x << endl;
  bool are_different = false;
  for (int i = 0; i < BIGARRAYSIZE; i += 4) {
    __m256i this_vec = this->load_to_m256i(i);
    __m256i that_vec = x.load_to_m256i(i);
    __m256i result = _mm256_cmpeq_epi64(this_vec, that_vec);
    for (int k = 0; k < 4; k++) {
      uint64_t cur_val = _mm256_extract_epi64(result, k);
      // cout << "cur_val = ";
      // print_in_binary(cur_val);
      // cout << endl;
      are_different = (cur_val != 0) ? are_different : true;
    }
  }
  // cout << "result is " << !(are_different) << endl;
  return !(are_different);
}

bool big_int::operator!=(const big_int x) const { return !(*this == x); }

big_int big_int::operator|(const big_int x) const {
  // cout << "Called |" << endl;
  big_int or_val = big_int();
  for (int i = 0; i < BIGARRAYSIZE; i += 4) {
    __m256i result =
        _mm256_or_si256(this->load_to_m256i(i), x.load_to_m256i(i));
    store_from_m256i(&or_val.num[i], result);
  }
  return or_val;
}

big_int big_int::operator&(const big_int x) const {
  // cout << "Called &" << endl;
  big_int and_val = big_int();
  for (int i = 0; i < BIGARRAYSIZE; i += 4) {
    __m256i result =
        _mm256_and_si256(this->load_to_m256i(i), x.load_to_m256i(i));
    store_from_m256i(&and_val.num[i], result);
  }
  return and_val;
}

big_int big_int::operator^(const big_int x) const {
  // cout << "Called ^" << endl;
  big_int xor_val = big_int();
  for (int i = 0; i < BIGARRAYSIZE; i += 4) {
    __m256i result =
        _mm256_xor_si256(this->load_to_m256i(i), x.load_to_m256i(i));
    store_from_m256i(&xor_val.num[i], result);
  }
  return xor_val;
}

big_int big_int::operator+(const big_int x) const {
  // cout << "Called +" << endl;
  bitset<WSIZE> this_bit, that_bit, result_bit, carry_bit;
  this_bit = this->to_bitset();
  that_bit = x.to_bitset();

  for (int i = 0; i < WSIZE; i++) {
    result_bit[i] = carry_bit[i] ^ this_bit[i] ^ that_bit[i];
    if ((this_bit[i] & that_bit[i]) ||
        ((this_bit[i] | that_bit[i]) & carry_bit[i])) {
      if (i < WSIZE - 1) {
        carry_bit[i + 1] = 1;
      }
    }
  }
  return big_int(result_bit);
}

big_int big_int::operator-() const {
  // cout << "Called - (unitary)" << endl;
  return ~(*this) + 1;
}

big_int big_int::operator-(const big_int x) const {
  // cout << "Called - (binary)" << endl;
  return (*this) + (-x);
}

big_int big_int::operator<<(const int x) const {
  // cout << "Called <<" << endl;
  return big_int(this->to_bitset() << x);
}

big_int big_int::operator>>(const int x) const {
  // cout << "Called >>" << endl;
  return big_int(this->to_bitset() >> x);
}

void big_int::operator+=(big_int x) { *this = (*this) + x; }

void big_int::operator-=(big_int x) { *this = (*this) - x; }

void big_int::operator|=(big_int x) { *this = (*this) | x; }

void big_int::operator&=(big_int x) { *this = (*this) & x; }

void big_int::operator^=(big_int x) { *this = (*this) ^ x; }

big_int big_int::operator*(const big_int x) const {
  // cout << "Called *" << endl;
  big_int res = big_int(0);

  for (int i = 0; i < WSIZE; i++) {
    if (x.get_kth_bit(i)) {
      res += (big_int((*this) << i));
    }
  }

  return res;
}

void big_int::operator*=(big_int x) { *this = (*this) * x; }

bitset<WSIZE> big_int::to_bitset() const {
  // cout << "Called to_bitset()" << endl;
  bitset<WSIZE> bits;
  for (int i = 0; i < WSIZE; i++) {
    bits[i] = this->get_kth_bit(i);
  }
  return bits;
}

ostream &operator<<(ostream &out, const big_int &bi) {
  for (int i = PSIZE - 1; i >= 0; i--) {
    out << bi.get_kth_bit(i);
  }

  return out;
}

#else

#include <iostream>
#include "big_int.hpp"

using namespace std;

big_int::big_int(int x) { bs = x; }

big_int::big_int(const bitset<WSIZE> &b) { bs = b; }

int big_int::to_int() const {
  int x = bs.to_ulong();
  return x;
}

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

big_int big_int::operator*(const big_int x) const {
  big_int res;

  for (int i = 0; i < WSIZE; i++) {
    if (x.bs[i]) {
      res += (big_int((*this) << i));
    }
  }

  return res;
}

ostream &operator<<(ostream &out, const big_int &bi) {
  for (int i = PSIZE - 1; i >= 0; i--) {
    out << bi.get_kth_bit(i);
    if ((PSIZE - 1 - i) % PINTERV == PINTERV - 1) {
      out << " ";
    }
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

#endif
