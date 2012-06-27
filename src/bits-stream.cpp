/* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php */
#include <stdint.h>
#include <string.h>
#include "bits-stream.h"


namespace bits {

  bitstream::bitstream(unsigned char *data) : buffer(data), offset(0) {
  }

  unsigned char * bitstream::ptr() { return buffer; }
  unsigned char * bitstream::current() { return buffer + position()/8; }
  bool bitstream::aligned() { return position()%8 == 0; }

  void bitstream::seek(int position) { offset = position; }
  void bitstream::rewind() { offset = 0; }
  void bitstream::skip(int bits) { offset += bits; }
  unsigned bitstream::position() { return offset; } 


  void bitstream::peekstring(unsigned char *dst, int numbits) {
    unsigned current_offset = position();
    if ( position()%8 || numbits%8 ) {
      while (numbits > 0) {
	*(dst++) = read<unsigned> ( std::min(numbits,8) );
	numbits -= 8;
      }
      seek(current_offset);
    } else {
      memcpy (dst, (buffer + current_offset/8), numbits/8);
    }
  }

  std::string bitstream::peekstring(int numbits) {
    unsigned char dst[numbits/8 + (numbits%8 ? 2 : 1)];
    peekstring (dst, numbits);
    dst[sizeof(dst)-1] = 0;
    return std::string ( (char *) dst );
  }

  void bitstream::readstring(unsigned char *dst, int numbits) {
    peekstring(dst, numbits);
    skip(numbits);
  }

  std::string bitstream::readstring(int numbits) {
    std::string s = peekstring(numbits);
    skip(numbits);
    return s;
  }

  void bitstream::readstring_at(unsigned char *dst, int offset, int numbits) {
    unsigned current_offset = position();
    seek(offset);
    peekstring(dst, numbits);
    seek(current_offset);
  }

  void bitstream::writestring(int numbits, const unsigned char *src) {
    if ( position()%8 || numbits%8 ) {
      /* if the write is not aligned, write byte by byte (slowish) */
      while (numbits > 0) {
	write<unsigned> ( std::min(numbits, 8), *(src++) );
	numbits -= 8;
      }
    } else {
      /* otherwise just copy the buffer */
      memcpy (buffer+(position()/8), src, numbits/8 );
      skip (numbits);
    }
  }

  void bitstream::writestring(std::string s, int max_bytes) {
    writestring ( std::min(max_bytes*8, (int) s.size() * 8), (const unsigned char *) s.c_str() );
    if (max_bytes > s.size()) zero ( (max_bytes - s.size()) * 8 );
  }

  void bitstream::writestring(std::string s) {
    writestring (s, s.size() );
  }

  void bitstream::zero (int numbits) {
    memset (numbits, 0);
  }

  void bitstream::memset(int numbits, unsigned char value ) {
    if ( position()%8 || numbits%8 ) {
      /* if the write is not aligned, write byte by byte (slowish) */
      while (numbits > 0) {
	write<unsigned> ( std::min(numbits, 8), value );
	numbits -= 8;
      }
    } else {
      /* otherwise just copy the buffer */
      ::memset (current(), value, numbits/8 );
      skip (numbits);
    }
  }

  void bitstream::writestring_at(int offset, int numbits, const unsigned char *src) {
    unsigned current_offset = position();
    seek(offset);
    writestring ( numbits, src );
    seek(current_offset);
  }

  void bitstream::writestring_at(int offset, std::string s, int max_bytes) {
    unsigned current_offset = position();
    seek(offset);
    writestring ( s, max_bytes );
    seek(current_offset);
  }

  void bitstream::writestring_at(int offset, std::string s) {
    writestring_at (offset, s, s.size() );
  }

}
