//
//  b-tree.hpp
//  Fusion Tree
//

#ifndef b_tree_hpp
#define b_tree_hpp

#include <stdio.h>

#include "fusiontree.hpp"

class b_node {
 private:
  fusiontree *key_tree;

  int K;

  b_node **children;

  bool is_leaf;

  void print_node() const;

 public:
  b_node(vector<big_int> &keys, environment &env, int K_);
  ~b_node();

  const big_int get_predecessor(const big_int &x) const;
};

class b_tree {
 private:
  b_node *root;

 public:
  b_tree(vector<big_int> &keys, environment &env, int K_);

  const big_int get_predecessor(const big_int &x) const;
};

#endif /* b_tree_hpp */
