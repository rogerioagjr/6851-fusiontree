//
//  fusiontree.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/23/18.
//

#ifndef fusiontree_hpp
#define fusiontree_hpp

#include <stdio.h>

#include <vector>

#include "big_int.hpp"
#include "bit_operations.hpp"
#include "constants.hpp"

using namespace std;

class fusiontree{
	
private:
	
	int k; // number of integers
	big_int mem; // sketched integers
	int q; // word size
	
	big_int v[K]; // real integers
	int sz; // size of tree
	
	big_int m; // integer m
	int m_idx[K]; // array with the m_i indexes
	big_int sketch_mask; // mask of all the m_i+b_i sums
	
	int r; // number of important bits
	big_int b; // mask of important bits
	int ibit[K]; // indexes of the important bits
	
	int w; // word size
	
	// add numbers from a vector to array v
	void add_in_array(vector<big_int> &v_);
	
	// finds the important bits of a set of integers
	void find_important_bits();
	
	// finds an integer m and sketch_mask to be used for sketching
	void find_m();
	
	// sets the variable mem that will keep the sketched numbers
	void set_mem();
	
public:
	
	// returns the val of mem
	const big_int mem_val() const;
	
	// returns the mask sketch_mask
	const big_int sketch_mask_val() const;
	
	// returns the value of r
	const int r_val() const;
	
	// returns the value of b
	const big_int b_val() const;
	
	// returns the value of b_i
	const int b_pos(int i) const;
	
	// returns the value of m
	const big_int m_val() const;
	
	// returns the value of m_i
	const int m_pos(int i) const;
	
	// returns the number of integers stored
	const int size() const;
	
	// returns the number in a given position in the tree
	const big_int pos(int i) const;
	
	// return a bitmask with all the important bits turned on
	const big_int important_bits() const;
	
	// returns the approximate sketch, in the fusion tree, of a given number
	const big_int approximate_sketch(const big_int &x) const;
	
	// v_ is a vector with the integers to be stored
	fusiontree(vector<big_int> &v_);
	
	// checks if the sketches of the integers are in ascending order
	const bool sketch_ok() const;
};

// prints all the numbers, in binary form, in a fusion tree
ostream &operator << (ostream &out, const fusiontree &t);

#endif /* fusiontree_hpp */
