//
//  main.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#include <iostream>
#include "fusiontree.hpp"
#include "big_int.hpp"
#include "test_generator.hpp"
#include "b-tree.hpp"

using namespace std;

int main() {
  vector<big_int> v;

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
  }
  
  test();
  
  return 0;
}
