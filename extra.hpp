//
//  extra.hpp
//
//
//  Created by Rogerio Aristida Guimaraes Junior on 8/25/18.
//

#ifndef extra_hpp
#define extra_hpp

#include <stdio.h>
#include <iostream>
#include "big_int.hpp"

int get_kth_bit(const big_int &bi, int k);

int clz(big_int const &x);

int most_significant_bit(big_int const &x);

// find the longest common prefix between two big_ints
const int first_diff(big_int const &x, big_int const &y);

#endif /* extra_hpp */
