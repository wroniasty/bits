/** -*- mode: c++ -*-
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */
#include <iostream>
#include <string>
#include <cpptest.h>
#include <string.h>
#include <math.h>

#include "bits.h"
#include "bits-stream.h"


using std::cout; using std::endl; using std::cerr;

class BitsTestSuite : public Test::Suite {
public:
  BitsTestSuite() {
    TEST_ADD(BitsTestSuite::put_test);
    TEST_ADD(BitsTestSuite::byte_test);
    TEST_ADD(BitsTestSuite::stream_test);
    TEST_ADD(BitsTestSuite::stream_memset_test);
  }

private:
  void put_test() {
    put_test_t<unsigned char> ( "unsigned char" );
    put_test_t<unsigned> ( "unsigned" );
    put_test_t<unsigned short> ( "unsigned short" );
    put_test_t<unsigned long> ( "unsigned long" );
    put_test_t<unsigned long long> ( "unsigned long long" );
  }

  void byte_test() {
    unsigned char buffer[12];
    memset(buffer, 0, sizeof(buffer) );

    bits::setbitbuffer<unsigned> ( buffer, 0, 8, 0x7f );
    TEST_ASSERT ( buffer[0] = 0x7f );

    bits::setbitbuffer<unsigned> ( buffer, 0, 4, 0x0 );
    TEST_ASSERT ( buffer[0] = 0xf );

    bits::setbitbuffer<unsigned> ( buffer, 4, 8, 0xff );
    TEST_ASSERT ( buffer[0] = 0xf );
    TEST_ASSERT ( buffer[1] = 0xf0 );

    bits::setbitbuffer<unsigned> ( buffer, 7, 32, 0xffffffff );
    TEST_ASSERT ( buffer[0] = 0x1 );
    TEST_ASSERT ( buffer[1] = 0xff );
    TEST_ASSERT ( buffer[4] = 0xfe );
  }

  template <class T> void put_test_t (const char *msg) {
    unsigned char buffer[sizeof(T)*10];

    for (int offset=0; offset < sizeof(T); offset++) {
      for (int numbits=9; numbits<=sizeof(T); numbits++) {
	T v = 0x7f7f7f7f & (unsigned) (pow(2,numbits)-1); 	
	memset(buffer, 0xff, sizeof(buffer));
	bits::setbitbuffer<T> ( buffer, offset, numbits, v);
	TEST_ASSERT_MSG( bits::getbitbuffer<T> (buffer, offset, numbits) == v, msg );
      }
    }

    for (int offset=0; offset < sizeof(T); offset++) {
      for (int numbits=1; numbits<=sizeof(T); numbits++) {
	memset(buffer, 0xff, sizeof(buffer));
	bits::setbitbuffer<T> ( buffer, offset, numbits, 0);
	TEST_ASSERT_MSG( bits::getbitbuffer<T> (buffer, offset, numbits) == 0, msg );
      }
    }    

    for (int offset=0; offset < sizeof(T); offset++) {
      for (int numbits=2; numbits<=sizeof(T); numbits++) {
	memset(buffer, 0x0, sizeof(buffer));
	bits::setbitbuffer<T> ( buffer, offset, numbits, 1);
	T v2 = bits::getbitbuffer<T> (buffer, offset, numbits);
	TEST_ASSERT_MSG( bits::getbitbuffer<T> (buffer, offset, numbits) == 1, msg );
      }
    }    

  }

  void stream_test() {
    unsigned char buffer[188];
    unsigned char str[3] = {1,2,3};
    bits::bitstream stream(buffer);

    memset(buffer, 0, sizeof(buffer) );
    
    stream.write(8, 'a');
    TEST_ASSERT ( stream.read_at<char>(0,8) == 'a' );
    stream.write(8, 'b');
    TEST_ASSERT ( stream.read_at<char>(8,8) == 'b' );
    stream.write(8, 'c');
    TEST_ASSERT ( stream.read_at<char>(16,8) == 'c' );
    stream.write(8, (unsigned char) 0);
    

    stream.seek(10);
    stream.writestring(24, str);
    
    TEST_ASSERT ( stream.read_at<unsigned char>(10,8) == 1 );
    TEST_ASSERT ( stream.read_at<unsigned char>(18,8) == 2 );
    TEST_ASSERT ( stream.read_at<unsigned char>(26,8) == 3 );

  }

  void stream_memset_test() {
    unsigned char buffer[16];
    bits::bitstream stream(buffer);

    memset(buffer, 0xff, sizeof(buffer) );

    stream.zero (sizeof(buffer)*8 );
    for (int i=0;i<sizeof(buffer);i++) TEST_ASSERT ( stream.read_at<char> (i*8, 8) == 0 );

    stream.rewind();
    stream.memset ( sizeof(buffer)*8, 170 );
    for (int i=0;i<sizeof(buffer);i++) {
      TEST_ASSERT ( stream.read_at<unsigned char> (i*8, 8) == 170 );
    }
    
    memset(buffer, 0xff, sizeof(buffer) );
    stream.rewind();
    stream.skip(4);
    stream.memset ( (sizeof(buffer) - 1)*8, 170 );
    for (int i=0;i<sizeof(buffer)-1;i++) TEST_ASSERT ( stream.read_at<unsigned char> (i*8+4, 8) == 170 );

  }

};


int main(int argc, char *argv[]) {
  BitsTestSuite bitstest;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return bitstest.run(output);
}
