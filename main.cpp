//
//  main.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#include <iostream>
#include "fusiontree.hpp"

using namespace std;

int main() {
  vector<big_int> v;

  int q;
  cin >> q;

  for (int i = 0; i < q; i++) {
    int k;
    cin >> k;

    v.push_back(k);
  }

  fusiontree t(v);

  cout << t.size() << endl;

  cout << t << endl;

  /*for (int i = 0; i < t.size(); i++) {
    cout << t.approximate_sketch(t.pos(i)) << endl;
  }

  cout << "r: " << t.r_val() << endl;

  cout << "bitmask: " << t.important_bits() << endl;

  cout << "sketch_mask: " << t.sketch_mask_val() << endl;

  cout << "m: " << t.m_val() << endl;

  for (int i = 0; i < t.r_val(); i++) {
    cout << "m[" << i << "]=" << t.m_pos(i) << ", b[" << i << "]=" << t.b_pos(i)
         << endl;
  }*/

  cout << string(t.sketch_ok() ? "correct" : "error") << endl;

  cout << "mem: " << t.mem_val() << endl;
}
