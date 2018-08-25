//
//  main.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#include <iostream>
#include "b-tree.hpp"
#include "big_int.hpp"
#include "bit_operations.hpp"
#include "fusiontree.hpp"
#include "test_generator.hpp"

using namespace std;

int main() {
  /*vector<big_int> v;

  int n;
  cin >> n;

  for (int i = 0; i < n; i++) {
    int k;
    cin >> k;

    v.push_back(k);
  }

  b_tree my_b_tree = b_tree(v);

  int q;
  cin >> q;

  for (int i = 0; i < q; i++) {
    int k;
    cin >> k;
    big_int k_ = k;
    big_int pred = my_b_tree.get_predecessor(k_);

    cout << "predecessor(" << k << ") = " << pred << endl;
  }*/

  /*bit_operations_initialize();

  cout << big_int(1193) << endl;

  cout << fast_most_significant_bit(1193) << endl;*/

  // cout << big_int(141) << endl;

  // fast_most_significant_bit(141);

  public_test();

  // test_big_int();

  /*fusiontree t = fusiontree(v);

  int q;
  cin >> q;

  for(int i=0;i<q;i++){

    int x;
    cin >> x;

    cout << t.find_predecessor(x) << endl;

    cout << t << endl;

    cout << t.approximate_sketch(t.pos(0)) << endl;
    cout << t.approximate_sketch(x) << endl;
  }*/

  return 0;
}
