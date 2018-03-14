//
//  constants.hpp
//  Fusion Tree
//
//  Created by Rogerio Aristida Guimaraes Junior on 1/31/18.
//

#ifndef constants_hpp
#define constants_hpp

#include <stdio.h>

/*#define WSIZE 330  // Size the big_int must have - O(K^5+K^4)
#define WVAR 243  // Value of w, the maximum number of bits of an element of a
fusion tree #define PSIZE 20  // Number of bits printed in cout << big_int
#define K 3       // Maximum size of a fusion tree - O(WVAR^(1/5))*/

#define WSIZE 6300  // Size the big_int must have - O(max(K^5+K^4,w+sqrt(w))
#define WVAR \
  3136  // Value of w, the maximum number of bits of an element of a fusion tree
#define SQRTW 56    // Value of sqrt(w), necessary for most significant bit
#define PSIZE 112   // Number of bits printed in cout << big_int
#define PINTERV 56  // Number of bits in printing intervals
#define K 5         // Maximum size of a fusion tree - O(WVAR^(1/5))

#endif /* constants_hpp */
