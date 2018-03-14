//
//  test_generator.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 2/18/18.
//

#ifndef test_generator_hpp
#define test_generator_hpp

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "big_int.hpp"
#include "bit_operations.hpp"
#include "constants.hpp"
#include "fusiontree.hpp"

using namespace std;

big_int random_big_int(int q = WSIZE) {
  bitset<WSIZE> b;

  for (int i = 0; i < q; i++) {
    b[i] = (rand() % 2);
  }

  return big_int(b);
}

bool is_in_vector(big_int &x, vector<big_int> &v) {
  for (int j = 0; j < v.size(); j++) {
    if (v[j] == x) {
      return true;
    }
  }
  return false;
}

int find_predecessor_in_vector(big_int &x, vector<big_int> &v) {
  for (int i = 0; i < v.size(); i++) {
    if (v[i] > x) return i - 1;
  }

  return (v.size() - 1);
}

int test() {
  srand(1998);

  int n, sz, m, q;
  cin >> n >> sz >> m >> q;

  bool error = false;

  for (int t = 0; !error and t < n; t++) {
    vector<big_int> v;

    for (int i = 0; i < sz; i++) {
      big_int x;

      do {
        x = random_big_int(q);
      } while (is_in_vector(x, v));

      v.push_back(x);
    }

    sort(v.begin(), v.end());

    fusiontree ft = fusiontree(v);

    // TEST 1 - check if the elements are inside the tree

    for (int i = 0; i < sz; i++) {
      if (ft.find_predecessor(v[i]) != i) {
        // if (ft.pos(i) != v[i]) {

        cout << "input " << t << endl;
        cout << "error, TEST 1 for following integers:" << endl;

        cout << ft.find_predecessor(v[i]) << "!=" << i << endl;

        cout << ft << endl;

        cout << ft.r_val() << " " << ft.k_val() << endl;
        cout << "ibit=";
        for (int j = 0; j < ft.r_val(); j++) {
          cout << ft.b_pos(j) << " ";
        }

        for (int j = 0; j < sz; j++) {
          big_int x = ft.pos(j);
          for (int z = ft.r_val() - 1; z >= 0; z--) {
            if ((x & (big_int(1) << ft.b_pos(z))) != 0) {
              cout << 1;
            } else {
              cout << 0;
            }
          }
          cout << "\n";
        }

        cout << endl;

        error = true;

        break;
      }
    }

    if (error) break;

    // TEST 2 - check predecessor of random numbers
    for (int i = 0; i < m; i++) {
      big_int x = random_big_int(q);

      if (ft.find_predecessor(x) != find_predecessor_in_vector(x, v)) {
        cout << "error, TEST 2 for following integers:" << endl << ft << endl;

        cout << "searching for:" << endl << x << endl;

        cout << "fusion tree says " << ft.find_predecessor(x)
             << " when it should say " << find_predecessor_in_vector(x, v)
             << endl;

        cout << string(ft.sketch_ok() ? "correct" : "error") << endl;

        cout << ft.important_bits() << endl << endl;

        for (int i = 0; i < K; i++)
          cout << ft.approximate_sketch(ft.pos(i)) << endl;

        cout << endl << ft.approximate_sketch(x) << endl;

        error = true;

        break;
      }
    }
  }

  if (!error) cout << "OK" << endl;

  return 0;
}

int diff_test() {
  // cout << fast_most_significant_bit(big_int(0)) << " "
  //     << most_significant_bit(big_int(0)) << endl;

  bit_operations_initialize();

  int n, q;
  cin >> n >> q;

  bool mistakes = false;

  for (int t = 0; t < n; t++) {
    big_int x;

    x = random_big_int(q);

    if (most_significant_bit(x) != fast_most_significant_bit(x)) {
      cout << "error" << endl << "input " << t << endl;

      cout << "x = " << x << endl;

      cout << "says: " << fast_most_significant_bit(x) << endl;
      cout << "should say: " << most_significant_bit(x) << endl;

      mistakes = true;
      break;
    }
  }

  if (!mistakes) {
    cout << "No mistakes!" << endl;
  }
}

#endif
