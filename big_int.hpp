//
//  big_int.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#ifndef big_int_hpp
#define big_int_hpp

#include <stdio.h>

#include <bitset>

#define PSIZE 4000   // Number of bits printed in cout << big_int
#define PINTERV 100  // Number of bits in printing intervals
#define WSIZE 4000   // Size the big_int must have - O(max(K^5+K^4,w+sqrt(w))

using namespace std;

class big_int {
 private:
  bitset<WSIZE> bs;

 public:
  int word_size() const;

  big_int(int x = 0);

  big_int(const bitset<WSIZE> &b);

  int to_int() const;

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

  big_int operator*(const big_int x) const;
};

ostream &operator<<(ostream &out, const big_int &bi);

#endif /* big_int_hpp */
