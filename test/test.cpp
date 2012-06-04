#include <iostream>
#include <string>
#include <cpptest.h>
#include <string.h>
#include <math.h>

#include "bits.h"
#include "bits-stream.h"


//using namespace bits;
using std::cout; using std::endl; using std::cerr;

class BitsTestSuite : public Test::Suite {
public:
  BitsTestSuite() {
    TEST_ADD(BitsTestSuite::put_test);
    TEST_ADD(BitsTestSuite::byte_test);
    TEST_ADD(BitsTestSuite::stream_test);
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
};


int main(int argc, char *argv[]) {
  BitsTestSuite bitstest;
  Test::TextOutput output(Test::TextOutput::Verbose);
  return bitstest.run(output);

  /*
  unsigned char buffer[24];
  memset(buffer, 1+4+16+64, sizeof(buffer) );
  for (size_t s=sizeof(buffer);s>0;s--) buffer[s-1]=(1+4+16+64);
  
  //  setbitbuffer (buffer, 0, 32, 0xffffffff );
  cout << "B2 < " << bits::binstr(buffer, 8 ) << endl;
  bits::setbitbuffer<unsigned long long> (buffer, 2, 8, 0x1);
  cout << "T1 < " << bits::binstr( bits::getbitbuffer<unsigned long long> (buffer, 0, 64) ) << endl;
  return 0;

  for (int offset=0;offset<40;offset++) {
    for (size_t s=sizeof(buffer);s>0;s--) buffer[s-1]=0xff;
    bits::setbitbuffer<unsigned int> (buffer, offset, 31, 0x0);
    cout << "B2 < " << bits::binstr(buffer, sizeof(buffer) ) << endl;
  }

  return 1;

  return bitstest.run ( output );
  */
}
