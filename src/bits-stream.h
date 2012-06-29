/** -*- mode: c++ -*- 
 *  Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php 
 */
#ifndef __BITS__BITS_STREAM_H
#define __BITS__BITS_STREAM_H 1
#include <stdint.h>
#include <string>
#include <iostream>

#include "bits.h"

namespace bits {

  class bitstream {
    unsigned offset;
    unsigned char * buffer;
  public:
    bitstream(unsigned char *data);

    unsigned char * ptr();
    unsigned char * current();
    bool aligned();

    void seek(int position);
    void rewind();
    void skip(int bits);
    unsigned position();

    template<class T> T peek(int numbits) {
      return getbitbuffer<T>(buffer, offset, numbits);
    }

    template<class T> T read(int numbits) {
      T v = getbitbuffer<T>(buffer, offset, numbits);
      offset += numbits;
      return v;
    }

    template<class T> T read_at(int offset, int numbits) {
      return getbitbuffer<T>(buffer, offset, numbits);
    }

    void readstring_at(unsigned char *dst, int offset, int numbits);
    void readstring(unsigned char *dst, int numbits);
    std::string readstring(int numbits);
    void peekstring(unsigned char *dst, int numbits);
    std::string peekstring(int numbits);

    template<class T> void write(int numbits, T v) {
      BITS_T_ASSERT(T);
      setbitbuffer<T> (buffer, offset, numbits, v);
      offset += numbits;
    }

    template<class T> void write_at(int offset, int numbits, T v) {
      BITS_T_ASSERT(T);
      setbitbuffer<T> (buffer, offset, numbits, v);
    }
 
    void writestring_at(int offset, int numbits, const unsigned char *v); 
    void writestring_at(int offset, std::string s);
    void writestring_at(int offset, std::string s, int max_bytes);
    void writestring(int numbits, const unsigned char *v); 

    void writestring(std::string s);
    void writestring(std::string s, int max_bytes);

    void zero (int numbits);
    void memset(int numbits, unsigned char value );
  };

}

#endif
