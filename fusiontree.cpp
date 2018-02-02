//
//  fusiontree.cpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/23/18.
//

#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "big_int.cpp"
#include "bit_operations.cpp"
#include "constants.cpp"

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

public:
	
	// returns the val of mem
	const big_int mem_val() const{
		
		return mem;
	}
	
	// returns the mask sketch_mask
	const big_int sketch_mask_val() const{
		
		return sketch_mask;
	}
	
	// returns the value of r
	const int r_val() const{
		
		return r;
	}
	
	// returns the value of b
	const big_int b_val() const{
		
		return b;
	}
	
	// returns the value of b_i
	const int b_pos(int i) const{
		
		return ibit[i];
	}
	
	// returns the value of m
	const big_int m_val() const{
		
		return m;
	}
	
	// returns the value of m_i
	const int m_pos(int i) const{
		
		return m_idx[i];
	}
	
	// returns the number of integers stored
	const int size() const{
		
		return sz;
	}
	
	// returns the number in a given position in the tree
	const big_int pos(int i) const{
		
		return v[i];
	}
	
	// return a bitmask with all the important bits turned on
	const big_int important_bits() const{
		
		return b;
	}
	
	// add numbers from a vector to array v
	void add_in_array(vector<big_int> &v_){
		
		for(int i=0;i<v_.size();i++){
			
			v[i]=v_[i];
			
			sort(v,v+size());
		}
		
		sz=v_.size();
	}
	
	// finds the important bits of a set of integers
	void find_important_bits(){
		
		if(size()==1) return;
		
		for(int i=1;i<size();i++){
			
			int diff_point=first_diff(pos(i),pos(0));
			
			for(int j=1;j<i;j++){
				
				int temp=first_diff(pos(i),pos(j));
				
				if(temp<diff_point) diff_point=temp;
			}
			
			b|=(big_int(1)<<(diff_point));
		}
		
		for(int i=0;i<WSIZE;i++){
			
			if((b&(big_int(1)<<i))!=0){
				
				ibit[r]=i;
				r++;
			}
		}
	}
	
	// finds an integer m and sketch_mask to be used for sketching
	void find_m(){
		
		big_int tag;
		
		int r3=r*r*r;
		
		for(int i=0;i<r;i++){
			
			for(int j=0;j<r3;j++){
				
				if((tag&(big_int(1)<<j))==0){
					
					m_idx[i]=j;
					
					for(int k1=0;k1<r;k1++){
						
						for(int k2=0;k2<r;k2++){
							
							if((j+ibit[k1]-ibit[k2])>=0){
								
								tag|=(big_int(1)<<(j+ibit[k1]-ibit[k2]));
							}
						}
					}
					
					break;
				}
			}
		}
		
		for(int i=0;i<r;i++){
			
			m_idx[i]+=r3*((w-ibit[i]+i*r3)/r3);
			
			m|=(big_int(1)<<(m_idx[i]));
			
			sketch_mask|=(big_int(1)<<(ibit[i]+m_idx[i]));
		}
	}
	
	// returns the approximate sketch, in the fusion tree, of a given number
	big_int approximate_sketch(big_int x){
		
		return ((((x&b)*m) & sketch_mask) >> (ibit[0]+m_idx[0]));
	}
	
	// sets the variable mem that will keep the sketched numbers
	void set_mem(){
		
		int r4=r*r*r*r;
		
		for(int i=0;i<k;i++){
			
			mem|=(big_int(1) << ((i+1)*r4+i));
			mem|=(approximate_sketch(pos(k-1-i)) << i*(r4+1));
		}
	}
	
	// v_ is a vector with the integers to be stored
	fusiontree(vector<big_int> &v_){
		
		q=k=K;
		mem=0;
		w=WVAR;
		
		add_in_array(v_);
		
		find_important_bits();
		
		find_m();
		
		set_mem();
	}
	
	bool sketch_ok(){
		
		for(int i=1;i<size();i++){
			
			if(approximate_sketch(pos(i))<=approximate_sketch(pos(i-1))){
				
				return false;
			}
		}
		
		return true;
	}
};

// prints all the numbers, in binary form, in a fusion tree
ostream &operator << (ostream &out, const fusiontree &t){
	
	for(int i=0;i<t.size();i++){
		
		out << t.pos(i) << endl;
	}
	
	return out;
}
