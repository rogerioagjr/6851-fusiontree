//
//  bit_operations.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#ifndef bit_operations_hpp
#define bit_operations_hpp
#include <iostream>

#include <stdio.h>

#include "big_int.hpp"
#include "constants.hpp"

// bitmasks precaulculated to avoid use od <<
static big_int t_mask_1[WSIZE], t_mask_no_1[WSIZE], t_mask_no_0[WSIZE], F, M,
    SK, K_POT, SK_F, SK_MULT;

// calculates the basic bitmasks used in bit tricks
void bit_operations_initialize();

const big_int mask_1(const int &x);

const big_int mask_no_1(const int &x);

const big_int mask_no_0(const int &x);

// find the longest common prefix between two big_ints
const int first_diff(big_int const &x, big_int const &y);

// first step of fast_most_significant_bit
const int sqrtw_first_bit(big_int x);

// find the most significant bit of a big_int in O(1) in word RAM model
const int fast_most_significant_bit(big_int const &x);

// find the longest common prefix between two big_ints in O(1) in word RAM model
const int fast_first_diff(big_int const &x, big_int const &y);

#endif /* bit_operations_hpp */
