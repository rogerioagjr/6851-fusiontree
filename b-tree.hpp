//
//  b-tree.hpp
//  Fusion Tree
//

#ifndef b_tree_hpp
#define b_tree_hpp

#include <stdio.h>

#include "constants.hpp"
#include "fusiontree.hpp"

class b_node {
private:
  fusiontree* key_tree;

  b_node* children[K + 1];

  bool is_leaf;

  void print_node() const;

public:
  b_node(vector<big_int> &keys);

  const big_int get_predecessor_node(big_int &x) const;
};

class b_tree {
private:
  b_node* root;

public:
  b_tree(vector<big_int> &keys);

  const big_int get_predecessor(big_int &x) const;
};

#endif /* b_tree_hpp */
