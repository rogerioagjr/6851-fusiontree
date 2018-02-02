//
//  bit_operations.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#include "bit_operations.hpp"

#include "constants.cpp"

// find the longest common prefix betwwen two big_ints
int first_diff(big_int const &x, big_int const &y){
	
	return most_significant_bit(x^y);
}
