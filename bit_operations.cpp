//
//  bit_operations.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#include "bit_operations.hpp"

// initializes the tricks of the library
void bit_operations_initialize() {
  // calculates the basic bitmasks used in bit tricks
  for (int i = 0; i < WSIZE; i++) {
    t_mask_1[i] = (big_int(1) << i);
    t_mask_no_1[i] = (~big_int(1) << i);
    t_mask_no_0[i] = (~big_int(0) << i);
  }

  // Find the value of F
  for (int i = 0; i < SQRTW; i++) {
    F |= t_mask_1[SQRTW - 1 + i * SQRTW];
  }

  // Find the value of M
  for (int i = 0; i < SQRTW; i++) {
    M |= t_mask_1[WVAR - (SQRTW - 1) - i * SQRTW + i];
  }

  // Find the value of SK_F
  for (int i = 0; i < SQRTW; i++) {
    SK_F |= t_mask_1[SQRTW + i * (SQRTW + 1)];
  }

  // Find the value of SK
  for (int i = 0; i < SQRTW; i++) {
    SK |= t_mask_1[i * (SQRTW + 1)];
  }

  // Find the value of K_POT
  for (int i = 0; i < SQRTW; i++) {
    K_POT |= t_mask_1[SQRTW - i - 1 + i * (SQRTW + 1)];
  }

  // Find the value of SK_MULT
  for (int i = 0; i < SQRTW; i++) {
    SK_MULT |= t_mask_1[i * (SQRTW + 1)];
  }
}

const big_int mask_1(const int &x) { return t_mask_1[x]; }

const big_int mask_no_1(const int &x) { return t_mask_no_1[x]; }

const big_int mask_no_0(const int &x) { return t_mask_no_0[x]; }

// find the longest common prefix between two big_ints
const int first_diff(big_int const &x, big_int const &y) {
  return most_significant_bit(x ^ y);
}

// first step of fast_most_significant_bit
const int sqrtw_first_bit(big_int x) {
  x *= SK;
  x |= SK_F;

  x = x - K_POT;

  x &= SK_F;

  x *= SK_MULT;

  x = x >> ((SQRTW * SQRTW) + (SQRTW - 1));

  x &= ~mask_no_0(SQRTW + 1);

  return x.to_int() - 1;
}

// find the most significant bit of a big_int in O(1) in word RAM model
const int fast_most_significant_bit(big_int const &x) {
  big_int x_first_bits = x & F;

  // cout << "1: " << x_first_bits << endl;

  big_int x_remain = x ^ x_first_bits;

  // cout << "2: " << x_remain << endl;

  x_remain = F - x_remain;

  x_remain &= F;
  x_remain ^= F;

  // cout << "3: " << x_remain << endl;

  // cout << "4: " << F << endl;

  big_int x_clusters = x_remain | x_first_bits;

  x_clusters = ((x_clusters * M) >> WVAR) & (~mask_no_0(SQRTW));

  int right_cluster_idx = sqrtw_first_bit(x_clusters);

  big_int right_cluster =
      (x >> (right_cluster_idx * SQRTW)) & (~mask_no_0(SQRTW));

  int ans = right_cluster_idx * SQRTW + sqrtw_first_bit(right_cluster);

  if (ans < 0) {
    ans = -1;
  }

  return ans;
}

// find the longest common prefix between two big_ints in O(1) in word RAM model
const int fast_first_diff(big_int const &x, big_int const &y) {
  return fast_most_significant_bit(x ^ y);
}
