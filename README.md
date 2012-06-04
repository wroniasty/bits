bits
====

**bits** is a tiny C++ bitstring library. It allows reading and writing numerical values of defined bit size, at a defined
bit offset in a memory buffer. There are no restrictions imposed on the bit size or the bit offset, making it possible
to write values without paying attention to byte boundaries.

Bit operations assume values are stored with the MSB(most significant bit) first.

Say what?
---------

Suppose you have a 2-byte buffer, and you need to retrieve/update a 4-bit value located in bits 7,8 
of the first byte and bits 1,2 of the second.

    buffer: 00101100 11001010
    bits:         ^^ ^^        (offset==6, bit_size==4)

In this simple case the task may be accomplished by a combination of bit operations (<< >> &), but as `bit_size`
increases it becomes slightly more complex and unreadable.
**bits** hides all the bitwise operations:

    int value = bits::getbitbuffer<int> (buffer, 6, 4); // value == 3

The inverse operation is also possible:

    bits::setbitbuffer (buffer, 6, 4, 5);
    // before: 00101100 11001010
    // bits:         ^^ ^^
    // after:  00101101 01001010
    assert ( bits::getbitbuffer<int> (buffer, 6, 4) == 5 );

In practice
-----------

Bits' primary use case is parsing and building binary protocol packets. A `bits::bitstream` class is included in the library 
to make this task cleaner and faster. The example snippet below parses an MPEGTS packet header:

    unsigned char packetData[188];
    bits::bitstream packetStream(packetData);

    assert (packetStream.read<unsigned> (8) == 0x47);  
    //read automatically moves an internal offset
    bool TEI = packetStream.read<bool> (1);
    bool PUSI = packetStream.read<bool> (1);
    bool TP = packetStream.read<bool> (1);

    unsigned PID = packetStream.read<unsigned> (13);

    unsigned char payload[188 - packetStream.position()/8];
    packetStream.readstring (payload, 188*8 - packetStream.position()];

A packet may be re-created in a very similar fashion:

    packetStream.rewind();
    packetStream.write(8, 0x47);
    packetStream.write(1, TEI);
    packetStream.write(1, PUSI);
    packetStream.write(1, TP);
    packetStream.write(13, PID);
    packetStream.writestring (188*8 - packetStream.position(), payload);
 


Installing
----------

bits uses the scons build system, so to build the library unpack the source and run:

    $ scons

in the source folder. To install run:

    $ scons install

to uninstall:

    $ scons -c


**That's it!**

Thank you to: [QuasarDonkey](http://stackoverflow.com/users/1428922/quasardonkey)


