/* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php */
#include <stdint.h>
#include <string.h>
#include "bits-stream.h"


namespace bits {

  bitstream::bitstream(unsigned char *data) : buffer(data), offset(0) {
  }

  unsigned char * bitstream::ptr() { return buffer; }

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

  void bitstream::readstring(unsigned char *dst, int numbits) {
    peekstring(dst, numbits);
    skip(numbits);
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

  void bitstream::writestring_at(int offset, int numbits, const unsigned char *src) {
    unsigned current_offset = position();
    seek(offset);
    writestring ( numbits, src );
    seek(current_offset);
  }

}
