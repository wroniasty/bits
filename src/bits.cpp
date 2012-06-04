#include <stdint.h>
#include <string.h>
#include "bits.h"


namespace bits {

  std::string binstr(unsigned char v) {
    std::string s; unsigned int m = 128;
    while (m > 0) {
      s += (v & m) ? "1" : "0";
      m >>= 1;
    }
    return s;
  } 

  std::string binstr(const unsigned char * v, std::size_t size) {
    std::string s;
    while (size--) { s += binstr( *(v++) ); }
    return s;
  }

  unsigned char setbits (unsigned char c, int offset, int numbits, unsigned char v) {
    unsigned char stamp = (v << (8-numbits)), mask = (0xff << (8-numbits));
    stamp >>= offset; mask = ~(mask >>= offset);
    return (c & mask) | stamp; 
  }

}
