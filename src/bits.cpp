/* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php */
#include <stdint.h>
#include <string.h>
#include "bits.h"


namespace bits {
  std::string hexdump ( const unsigned char * buffer, int length, int line_size) {
        std::string dump;
        for (int i = 0; i < length; i+=line_size) {
            std::string _hex, _ascii;
            for (int j=i; j < i+line_size; j++) {
                _hex += boost::str ( boost::format("%02X ") % (int)buffer[j] );
                if (j%4 == 3) _hex += " ";
                _ascii += (buffer[j] >= ' ' && buffer[j] <= 'z') ? buffer[j] : '.';
            }
            dump = dump + _hex + "   " + _ascii + "\n"; //endl;
        }
        return dump;
  }
  
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
