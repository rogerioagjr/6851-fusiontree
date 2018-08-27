//
//  fusiontree.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/23/18.
//

#ifndef fusiontree_hpp
#define fusiontree_hpp

#include <stdio.h>
#include <memory>
#include <vector>

#include "big_int.hpp"

using namespace std;

class fusiontree {
 private:
  int word_size;     // Size of the type being used as big int, in bits
  int element_size;  // Size of the element of the fusion_tree, must be a square
  int sqrt_element_size;  // Value of sqrt(element_size), necessary for most
                          // significant bit
  int capacity;           // maximum number of integers in a fusion tree

  // bitmasks precaulculated to avoid use of <<
  big_int *t_mask_1, *t_mask_no_1, *t_mask_no_0;

  big_int F, M, SK, K_POT, SK_F, SK_MULT;

  int k;        // maximum number of integers in a fusion tree
  big_int mem;  // sketched integers

  big_int *v;  // real integers
  int sz;      // size of tree

  big_int k_mult;    // integer used by parallel comparison
  big_int diff_and;  // bitmask used in parallel comparison
  big_int pos_and;   // bitmask used in last step of parallel comparison

  big_int m;            // integer m
  int *m_idx;           // array with the m_i indexes
  big_int sketch_mask;  // mask of all the m_i+b_i sums

  int r;      // number of important bits
  big_int b;  // mask of important bits
  int *ibit;  // indexes of the important bits

  // calculates the basic bitmasks used in bit tricks
  void bit_operations_initialize();

  // access the basic bitmasks used in bit tricks
  const big_int mask_1(const int &x) const;
  const big_int mask_no_1(const int &x) const;
  const big_int mask_no_0(const int &x) const;

  // first step of fast_most_significant_bit
  const int sqrtw_first_bit(big_int x) const;

  // find the most significant bit of a big_int in O(1) in word RAM model
  const int fast_most_significant_bit(big_int const &x) const;

  // find the longest common prefix between two big_ints in O(1) in word RAM
  // model
  const int fast_first_diff(big_int const &x, big_int const &y) const;

  // add numbers from a vector to array v
  void add_in_array(vector<big_int> &v_);

  // finds the important bits of a set of integers
  void find_important_bits();

  // finds an integer m and sketch_mask to be used for sketching
  void find_m();

  // sets the variables used in parallel comparison
  void set_parallel_comparison();

  // returns the approximate sketch, in the fusion tree, of a given number
  const big_int approximate_sketch(const big_int &x) const;

  // returns an integer with O(w^(1/5)) sketches of x, separated by zeroes
  const big_int sketch_k(const big_int &x) const;

  // returns the index of the biggest k in the tree succh that
  // sketch(k)<=sketch(x)
  const int find_sketch_predecessor(const big_int &x) const;

 public:
  // returns the number of integers stored
  const int size() const;

  // returns the size of the elements in the fusion tree
  const int element_size_val() const;

  // returns the number in a given position in the tree
  const big_int pos(int i) const;

  // returns the index of the biggest k in the tree succh that k<=x
  // or -1 if there is no such k
  const int find_predecessor(const big_int &x) const;

  // fusiontree constructor
  // v_ is a vector with the integers to be stored
  fusiontree(vector<big_int> &v_, int k_ = 5, int word_size_ = 4000,
             int element_size_ = 3136);

  // fusiontree destructor
  ~fusiontree();
};

// prints all the numbers, in binary form, in a fusion tree
ostream &operator<<(ostream &out, const fusiontree &t);

#endif /* fusiontree_hpp */
