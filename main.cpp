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

  int n;
  cin >> n;

  for (int i = 0; i < n; i++) {
    int k;
    cin >> k;

    v.push_back(k);
  }

  fusiontree t(v);

  cout << t.size() << endl;

  cout << t << endl;

  cout << string(t.sketch_ok() ? "correct" : "error") << endl;

  cout << t.mem_val() << endl;

  int q;
  cin >> q;

  for (int i = 0; i < q; i++) {
    int x;
    cin >> x;

    cout << x << ":" << endl;

    cout << t.find_predecessor(x) << endl << endl;
  }
}
