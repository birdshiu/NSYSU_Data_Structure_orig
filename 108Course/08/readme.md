huffman coding
===
This is a huffman encoding file application.
You can use a.out or main.cpp singly. 
if you wnat to use this in debian environment, suggest you run setup.sh first!

# How to build
* Compile cpp/hpp to elf:
    * g++ -std=c++17 main.cpp tools.hpp huffmanInCpp.cpp huffmanInCpp.hpp -o huffman.elf
* Compile to so file for dynamic linking in python:
    * g++ -fPIC -rdynamic -shared -std=c++2a tools.hpp huffmanInCpp.cpp huffmanInCpp.hpp -o huffman.so
    * ref: https://stackoverflow.com/questions/145270/calling-c-c-from-python
* setup python environment
    * ./setup.sh

# How to use
* python(GUI):
    * python3 p8.py
* elf:
    * ./huffman.elf <file> compress
    * ./huffman.elf <file> decompress

# Header format 
 * little endian
 * header format(2 part):
    info:
        |originSize(4Bytes)|compressBitsLength (4Bytes)|IEEE-754 for compressRate(4Byte)|
        |codingTable.size(1Byte)|padding.length(1Byte)|
    codingtable:
        |uChar(1Byte)|codingLength(1Byte)|coding contents|

==Warning==
input file(to compress) MAX size is 512.0 MB