//
//  big_int.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#ifndef big_int_hpp
#define big_int_hpp

#include <stdio.h>

#include "constants.hpp"
#include <bitset>

using namespace std;

class big_int{
	
private:
	
	bitset<WSIZE> bs;
	
public:
	
	big_int(int x=0);
	
	big_int(bitset<WSIZE> b);
	
	int get_kth_bit(int k) const;
	
	big_int operator ~() const;
	
	big_int operator -() const;
	
	bool operator <(const big_int x) const;
	
	bool operator <=(const big_int x) const;
	
	bool operator >(const big_int x) const;
	
	bool operator >=(const big_int x) const;
	
	bool operator ==(const big_int x) const;
	
	bool operator !=(const big_int x) const;
	
	big_int operator <<(const int x) const;
	
	big_int operator >>(const int x) const;
	
	big_int operator |(const big_int x) const;
	
	big_int operator &(const big_int x) const;
	
	big_int operator ^(const big_int x) const;
	
	big_int operator +(const big_int x) const;
	
	big_int operator -(const big_int x) const;
	
	void operator +=(big_int x);
	
	void operator -=(big_int x);
	
	void operator |=(big_int x);
	
	void operator &=(big_int x);
	
	void operator ^=(big_int x);
	
	big_int operator *(const big_int x) const;
	
	void operator *=(big_int x);
};

ostream &operator << (ostream &out, const big_int &bi);

int clz(big_int const &x);

int most_significant_bit(big_int const &x);

#endif /* big_int_hpp */
