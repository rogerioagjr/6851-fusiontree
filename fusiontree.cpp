//
//  fusiontree.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/23/18.
//

#include "fusiontree.hpp"

#include <algorithm>
#include <cmath>

#include <iostream>

using namespace std;

// initializes the tricks of the library
environment::environment(int word_size_, int element_size_, int capacity_)
    : word_size(word_size_), element_size(element_size_), capacity(capacity_) {
  try {
    sqrt_element_size = sqrt(element_size);
    if (sqrt_element_size * sqrt_element_size != element_size) {
      throw(string("element_size is not a square"));
    }
    int capacity_to_4 = capacity * capacity * capacity * capacity;
    int capacity_to_5 = capacity_to_4 * capacity;
    if (capacity_to_5 > element_size) {
      throw(string("element_size is too small for the fusion tree capacity"));
    }

    if (capacity_to_5 + capacity_to_4 > word_size) {
      throw(string("word_size is too small for the fusion tree capacity"));
    }
  } catch (const string msg) {
    cerr << msg << endl;
  }

  shift_1 = new (big_int[word_size]);
  shift_neg_1 = new (big_int[word_size]);
  shift_neg_0 = new (big_int[word_size]);

  // calculates the basic bitmasks used in bit tricks
  for (int i = 0; i < word_size; i++) {
    shift_1[i] = (big_int(1) << i);
    shift_neg_1[i] = (~big_int(1) << i);
    shift_neg_0[i] = (~big_int(0) << i);
  }

  // Find the value of F
  for (int i = 0; i < sqrt_element_size; i++) {
    F = F | (shift_1[sqrt_element_size - 1 + i * sqrt_element_size]);
  }

  // Find the value of M
  for (int i = 0; i < sqrt_element_size; i++) {
    M = M | (shift_1[element_size - (sqrt_element_size - 1) -
                     i * sqrt_element_size + i]);
  }

  // Find the value of SK_F
  for (int i = 0; i < sqrt_element_size; i++) {
    SK_F = SK_F | (shift_1[sqrt_element_size + i * (sqrt_element_size + 1)]);
  }

  // Find the value of SK
  for (int i = 0; i < sqrt_element_size; i++) {
    SK = SK | (shift_1[i * (sqrt_element_size + 1)]);
  }

  // Find the value of K_POT
  for (int i = 0; i < sqrt_element_size; i++) {
    K_POT = K_POT |
            (shift_1[sqrt_element_size - i - 1 + i * (sqrt_element_size + 1)]);
  }

  // Find the value of SK_MULT
  for (int i = 0; i < sqrt_element_size; i++) {
    SK_MULT = SK_MULT | (shift_1[i * (sqrt_element_size + 1)]);
  }
}

environment::~environment() {
  delete[] shift_1;
  delete[] shift_neg_1;
  delete[] shift_neg_0;
}

// first step of fast_most_significant_bit
const int environment::sqrtw_first_bit(big_int x) const {
  x = x * SK;
  x = x | SK_F;

  x = x - K_POT;

  x = x & SK_F;

  x = x * SK_MULT;

  x = x >> ((element_size) + (sqrt_element_size - 1));

  x = x & (~shift_neg_0[sqrt_element_size + 1]);

  return x.to_int() - 1;
}

// find the most significant bit of a big_int in O(1) in word RAM model
const int environment::fast_most_significant_bit(big_int const &x) const {
  big_int x_first_bits = x & F;

  big_int x_remain = x ^ x_first_bits;

  x_remain = F - x_remain;

  x_remain = x_remain & F;
  x_remain = x_remain ^ F;

  big_int x_clusters = x_remain | x_first_bits;

  x_clusters =
      ((x_clusters * M) >> element_size) & (~shift_neg_0[sqrt_element_size]);

  int right_cluster_idx = sqrtw_first_bit(x_clusters);

  big_int right_cluster = (x >> (right_cluster_idx * sqrt_element_size)) &
                          (~shift_neg_0[sqrt_element_size]);

  int ans =
      right_cluster_idx * sqrt_element_size + sqrtw_first_bit(right_cluster);

  if (ans < 0) {
    ans = -1;
  }

  return ans;
}

// find the longest common prefix between two big_ints in O(1) in word RAM model
const int environment::fast_first_diff(big_int const &x,
                                       big_int const &y) const {
  return fast_most_significant_bit(x ^ y);
}

// add numbers from a vector to array v
void fusiontree::add_in_array(vector<big_int> &v_) {
  sz = v_.size();
  for (int i = 0; i < v_.size(); i++) {
    v[i] = v_[i];
  }
  std::sort(v, v + size());
}

// finds the important bits of a set of integers
void fusiontree::find_important_bits() {
  if (size() == 1) return;

  for (int i = 1; i < size(); i++) {
    int diff_point = my_env->fast_first_diff(pos(i), pos(0));

    for (int j = 1; j < i; j++) {
      int temp = my_env->fast_first_diff(pos(i), pos(j));

      if (temp < diff_point) diff_point = temp;
    }

    b = b | my_env->shift_1[diff_point];
  }

  for (int i = 0; i < my_env->word_size; i++) {
    if ((b & my_env->shift_1[i]) != 0) {
      important_bits[r] = i;
      r++;
    }
  }
}

// finds an integer m and sketch_mask to be used for sketching
void fusiontree::find_m() {
  int r3 = r * r * r;

  // set m
  big_int tag;

  for (int i = 0; i < r; i++) {
    for (int j = 0; j < r3; j++) {
      if ((tag & my_env->shift_1[j]) == 0) {
        m_indices[i] = j;

        for (int k1 = 0; k1 < r; k1++) {
          for (int k2 = 0; k2 < r; k2++) {
            if ((j + important_bits[k1] - important_bits[k2]) >= 0) {
              tag = tag | (my_env->shift_1[j + important_bits[k1] -
                                           important_bits[k2]]);
            }
          }
        }

        break;
      }
    }
  }

  // set sketch_mask
  for (int i = 0; i < r; i++) {
    m_indices[i] =
        m_indices[i] +
        (r3 * ((my_env->element_size - important_bits[i] + i * r3) / r3));

    m = m | my_env->shift_1[m_indices[i]];

    sketch_mask =
        sketch_mask | (my_env->shift_1[important_bits[i] + m_indices[i]]);
  }
}

// sets the variable mem that will keep the sketched numbers
void fusiontree::set_parallel_comparison() {
  int r4 = r * r * r * r;

  // set mem
  for (int i = 0; i < my_env->capacity; i++) {
    mem = mem | my_env->shift_1[(i + 1) * r4 + i];
    mem = mem |
          (approximate_sketch(pos(my_env->capacity - 1 - i)) << i * (r4 + 1));
  }

  // set k_mult
  for (int i = 0; i < my_env->capacity; i++) {
    k_mult = k_mult | my_env->shift_1[i * (r4 + 1)];
  }

  // set diff_and
  for (int i = 0; i < my_env->capacity; i++) {
    diff_and = diff_and | my_env->shift_1[(i + 1) * r4 + i];
  }

  // set pos_and
  for (int i = 0; i < r4; i++) {
    pos_and = pos_and | my_env->shift_1[i];
  }
}

// returns the approximate sketch, in the fusion tree, of a given number
const big_int fusiontree::approximate_sketch(const big_int &x) const {
  big_int ret =
      ((((x & b) * m) & sketch_mask) >> (important_bits[0] + m_indices[0]));
  return ret;
}

// returns an integer with O(element_size^(1/5)) sketches of x, separated by
// zeroes
const big_int fusiontree::sketch_k(const big_int &x) const {
  big_int ret = approximate_sketch(x) * k_mult;
  return ret;
}

// returns the index of the biggest k in the tree succh that
// sketch(k)<=sketch(x)
const int fusiontree::find_sketch_predecessor(const big_int &x) const {
  int r4 = r * r * r * r;

  big_int diff = mem - sketch_k(x);

  diff = diff & diff_and;

  diff = diff * k_mult;

  diff = diff >> ((my_env->capacity * r4) + (my_env->capacity - 1));

  diff = diff & pos_and;

  int answer = size() - diff.to_int() - 1;

  if (answer + 1 < size() and
      approximate_sketch(v[answer + 1]) == approximate_sketch(x)) {
    answer++;
  }

  return answer;
}

// returns the number of integers stored
const int fusiontree::size() const { return sz; }

// returns the number in a given position in the tree
const big_int fusiontree::pos(int i) const { return v[i]; }

// returns the index of the biggest k in the tree succh that k<=x
// or -1 if there is no such k
const int fusiontree::find_predecessor(const big_int &x) const {
  int idx1 = find_sketch_predecessor(x);
  int idx2 = idx1 + 1;

  int q1, q2;

  if (idx1 < 0) {
    q1 = -2;
  } else {
    q1 = my_env->fast_first_diff(v[idx1], x);
  }

  if (idx2 < size()) {
    q2 = my_env->fast_first_diff(v[idx2], x);
  } else {
    q2 = -2;
  }

  if (q1 == -1) {
    return idx1;
  }
  if (q2 == -1) {
    return idx2;
  }

  int answer = 0;

  big_int y;

  int q;

  if (q1 == -2) q = q2;
  if (q2 == -2) q = q1;
  if (q1 != -2 and q2 != -2) q = min(q1, q2);

  // if diff bit is 1
  if ((x & my_env->shift_1[q]) != 0) {
    y = x & my_env->shift_neg_1[q];
    y = y | (my_env->shift_1[q] - 1);

    answer = find_sketch_predecessor(y);
  }

  // if diff bit is 0
  else {
    y = x | my_env->shift_1[q];
    y = y & my_env->shift_neg_0[q];

    answer = find_sketch_predecessor(y);

    if (answer >= 0 and v[answer] > x) {
      answer--;
    }
  }

  return answer;
}

// v_ is a vector with the integers to be stored
fusiontree::fusiontree(vector<big_int> &v_, environment *my_env_) {
  my_env = my_env_;
  v = new (big_int[my_env->word_size]);
  m_indices = new (int[my_env->word_size]);
  important_bits = new (int[my_env->capacity]);
  mem = 0;
  r = 0;

  add_in_array(v_);

  find_important_bits();

  find_m();

  set_parallel_comparison();
}

fusiontree::~fusiontree() {
  delete[] v;
  delete[] m_indices;
  delete[] important_bits;
}

// prints all the numbers, in binary form, in a fusion tree
ostream &operator<<(ostream &out, const fusiontree &t) {
  for (int i = 0; i < t.size(); i++) {
    out << t.pos(i) << endl;
  }

  return out;
}
