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

// environment deconstructor
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

// add numbers from a vector to array elements
void fusiontree::add_in_array(vector<big_int> &elements_) {
  // sets variable sz, which keeps the size of the fusion tree
  sz = elements_.size();

  // copies all the elements from the vector to array elements
  for (int i = 0; i < elements_.size(); i++) {
    elements[i] = elements_[i];
  }

  // sorts the array elements in ascending order
  std::sort(elements, elements + size());
}

// finds the important bits of a set of integers
void fusiontree::find_important_bits() {
  // if the fusion tree has a single element, there are no important bits
  if (size() == 1) return;

  // find the first bit that differentiates each element from any other element
  // of the fusion tree
  for (int i = 1; i < size(); i++) {
    // for each element i, find where it first differentiates from the first
    // element
    int diff_point = my_env->fast_first_diff(pos(i), pos(0));

    // then check if there is any other element that differentiate from i in a
    // later bit
    for (int j = 1; j < i; j++) {
      // for each other element j
      // find where it first differentiates from i
      int temp = my_env->fast_first_diff(pos(i), pos(j));
      // if it is later then the first element, keep the index of such bit
      if (temp < diff_point) diff_point = temp;
    }

    // the first bit that differentiates element i from any other in the fusion
    // tree is an important bit, so add it in mask_important_bits
    mask_important_bits = mask_important_bits | my_env->shift_1[diff_point];
  }

  // keep all the important bits found in array important_bits and update the
  // value of important_bits_count
  for (int i = 0; i < my_env->word_size; i++) {
    // foir each bit position i in the word size given
    // if bit in position i is an important bit
    if ((mask_important_bits & my_env->shift_1[i]) != 0) {
      // add such position to array important_bits
      important_bits[important_bits_count] = i;
      // and update the value of important_bits_count
      important_bits_count++;
    }
  }
}

// finds an integer m, used to find the sketch of a numeber, and sketch_mask,
// used to extract the important bits from it
void fusiontree::find_m() {
  // precalculates the third power of the number of important bits
  int important_bits_count_to_3 =
      important_bits_count * important_bits_count * important_bits_count;

  // find the indices of the set bits in m
  // tag is a bitmask used to denote that a certain position cannot be used
  // anymore because it would cause a collision between the position of two
  // distinct important bits after the multiplication by m to find the sketch of
  // a number
  big_int tag;

  // for every important bit b_i we will find an integer m_i such that b_i+m_i
  // is distinct for all i, modulo capacity^3. If each m_i represents one set
  // bit in m, it garantees that each important bit of the number x we are
  // sketching will be taken to a different position in x*m
  for (int i = 0; i < important_bits_count; i++) {
    // for every important bit b_i
    for (int j = 0; j < important_bits_count_to_3; j++) {
      // we will search for the next position m_i available in m
      // such that b_i+m_i is unique
      if ((tag & my_env->shift_1[j]) == 0) {
        // if a position is not tagged, then it means there is no other pair
        // b_j, m_j such that b_j+m_j=b_i+m_i thus this position can be m_i
        m_indices[i] = j;

        // then we need to tag every single position p such that b_i+m_i=b_j+p,
        // for any pair of important bits b_i, b_j, so that p does not be chosen
        // as any m_j. This is the same of tagging every position p such that
        // m_i+b_i-b_j=p Thus, for every pair of important bits
        for (int k1 = 0; k1 < important_bits_count; k1++) {
          for (int k2 = 0; k2 < important_bits_count; k2++) {
            // We tag the value of m_i+b_i-b_j, if its positive
            if ((j + important_bits[k1] - important_bits[k2]) >= 0) {
              // adding a bit in the bitmask tag
              tag = tag | (my_env->shift_1[j + important_bits[k1] -
                                           important_bits[k2]]);
            }
          }
        }

        // it is garanteed that we will find enough all the values for all m_i
        // in the first capacity^3 bit positions.

        // after finding and m_i for b_i, we will break the loop and find
        // m_(i+1)
        break;
      }
    }
  }
  // we already have m_i+b_i distinct modulo capacity^3, now we have to spread
  // the values of m_i+b_i such that each of them is in a distinct interval of
  // size capacity^3, so that we can also maintain the order of the important
  // bits in the sketch of x
  for (int i = 0; i < important_bits_count; i++) {
    // adding i*capacity^3 to each m_i garantees that each m_i will be in a
    // distinct interval of capacity^3 bits. Also, m_i will be in interval i.
    // However, we want m_i+b_i to be in interval i, thus we must add
    // i*capacity^3-b_i. However it can be negative, so we will add, instead,
    // element_size-b_i+i*capacity^3, rounded down to the nearest multiple of
    // capacity^3
    m_indices[i] = m_indices[i] + (important_bits_count_to_3 *
                                   ((my_env->element_size - important_bits[i] +
                                     i * important_bits_count_to_3) /
                                    important_bits_count_to_3));
    // then we set up the bit m_i of m
    m = m | my_env->shift_1[m_indices[i]];

    // then, we have to set sketch_mask, which is a bitmask that allows us to
    // extract the important bits of x after we multiply x*m. It is easy to see
    // that each bit b_i will be gound in position b_i+m_i, thus we only have to
    // set a bit mask with those bits
    sketch_mask =
        sketch_mask | (my_env->shift_1[important_bits[i] + m_indices[i]]);
    // note that the distance between the first and last important bits is at
    // most capacity*(capacity^3)=capacity^4. Since element_size > capacity^5,
    // we can store all the sketches in a single element
  }
}

// sets the variable data that will keep the sketched numbers
void fusiontree::set_parallel_comparison() {
  int important_bits_count_to_4 = important_bits_count * important_bits_count *
                                  important_bits_count * important_bits_count;

  // set data
  for (int i = 0; i < my_env->capacity; i++) {
    data = data | my_env->shift_1[(i + 1) * important_bits_count_to_4 + i];
    data = data | (approximate_sketch(pos(my_env->capacity - 1 - i))
                   << i * (important_bits_count_to_4 + 1));
  }

  // set repeat_int
  for (int i = 0; i < my_env->capacity; i++) {
    repeat_int =
        repeat_int | my_env->shift_1[i * (important_bits_count_to_4 + 1)];
  }

  // set extract_interposed_bits
  for (int i = 0; i < my_env->capacity; i++) {
    extract_interposed_bits =
        extract_interposed_bits |
        my_env->shift_1[(i + 1) * important_bits_count_to_4 + i];
  }

  // set extract_interposed_bits_sum
  for (int i = 0; i < important_bits_count_to_4; i++) {
    extract_interposed_bits_sum =
        extract_interposed_bits_sum | my_env->shift_1[i];
  }
}

// returns the approximate sketch, in the fusion tree, of a given number
const big_int fusiontree::approximate_sketch(const big_int &x) const {
  big_int ret = ((((x & mask_important_bits) * m) & sketch_mask) >>
                 (important_bits[0] + m_indices[0]));
  return ret;
}

// returns an integer with O(element_size^(1/5)) sketches of x, separated by
// zeroes
const big_int fusiontree::sketch_k(const big_int &x) const {
  big_int ret = approximate_sketch(x) * repeat_int;
  return ret;
}

// returns the index of the biggest k in the tree succh that
// sketch(k)<=sketch(x)
const int fusiontree::find_sketch_predecessor(const big_int &x) const {
  int important_bits_count_to_4 = important_bits_count * important_bits_count *
                                  important_bits_count * important_bits_count;

  big_int diff = data - sketch_k(x);

  diff = diff & extract_interposed_bits;

  diff = diff * repeat_int;

  diff = diff >> ((my_env->capacity * important_bits_count_to_4) +
                  (my_env->capacity - 1));

  diff = diff & extract_interposed_bits_sum;

  int answer = size() - diff.to_int() - 1;

  if (answer + 1 < size() and
      approximate_sketch(elements[answer + 1]) == approximate_sketch(x)) {
    answer++;
  }

  return answer;
}

// returns the number of integers stored
const int fusiontree::size() const { return sz; }

// returns the number in a given position in the tree
const big_int fusiontree::pos(int i) const { return elements[i]; }

// returns the index of the biggest k in the tree succh that k<=x
// or -1 if there is no such k
const int fusiontree::find_predecessor(const big_int &x) const {
  int idx1 = find_sketch_predecessor(x);
  int idx2 = idx1 + 1;

  int q1, q2;

  if (idx1 < 0) {
    q1 = -2;
  } else {
    q1 = my_env->fast_first_diff(elements[idx1], x);
  }

  if (idx2 < size()) {
    q2 = my_env->fast_first_diff(elements[idx2], x);
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

    if (answer >= 0 and elements[answer] > x) {
      answer--;
    }
  }

  return answer;
}

// fusiontree constructor
// v_ is a vector with the integers to be stored
// my_env is the environment with the specifications of the fusion tree
fusiontree::fusiontree(vector<big_int> &elements_, environment *my_env_) {
  // set the values of the class variables
  // see class fusiontree in the header file for comments on each variable

  my_env = my_env_;  // keeps a pointer to the environment *my_env_

  // creates the arrays necessary for the fusion tree, allocating
  // dynamically because variable lenght arrays are forbidden as class members
  elements = new (big_int[my_env->word_size]);
  m_indices = new (int[my_env->word_size]);
  important_bits = new (int[my_env->capacity]);
  data = 0;
  important_bits_count = 0;

  // adds the elements of the array elements_, passed as a reference in the
  // first argument in the array elements, which is a class member, and keeps
  // the elements in the fusion tree
  add_in_array(elements_);

  // finds the important bits of the elements in the fusion tree,
  // and saves them in array important_bits, as well as a bitmask of them in
  // big_int mask_important_bits, and the number of important bits in
  // important_bits_count
  find_important_bits();

  // finds the value of big_int m, which is used in parallel comparison,
  // and the value of sketch_mask, the bitmask to extract the important bits
  // from the sketched integer
  find_m();

  // the last thing to be done to initialize a fusion tree
  set_parallel_comparison();
}

fusiontree::~fusiontree() {
  delete[] elements;
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
