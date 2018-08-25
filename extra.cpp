//
//  extra.cpp
//
//
//  Created by Rogerio Aristida Guimaraes Junior on 8/25/18.
//

#include "extra.hpp"

int get_kth_bit(const big_int &bi, int k) {
  if ((bi & (big_int(1) << k)) != big_int(0))
    return 1;
  else
    return 0;
}

int clz(big_int const &x) {
  int res = 0;

  for (int i = WSIZE - 1; i >= 0; i--) {
    if (get_kth_bit(x, i) == 0) {
      res++;
    }

    else {
      break;
    }
  }

  return res;
}

int most_significant_bit(big_int const &x) { return WSIZE - clz(x) - 1; }

// find the longest common prefix between two big_ints
const int first_diff(big_int const &x, big_int const &y) {
  return most_significant_bit(x ^ y);
}
