Using cpp to speed up the encoding efficient, and actually Cpp is my main programming language

Will remove the huffmanCoding.py
The huffmanCoding.py is just for tersting

--Cpp is awesome--

compile info
// remind me compiling cpp to so(cdll)
g++-9 -std=c++2a tools.hpp huffmanInCpp.cpp huffmanInCpp.hpp -c -fPIC
g++-9 -shared huffmanInCpp.o -o huffman.so
https://stackoverflow.com/questions/145270/calling-c-c-from-python


# Header format 
 little endian
 header format:
 |originSize(4Bytes)|compressBitsLength (4Bytes)|IEEE-754 for compressRate(4Byte)|
 |codingTable.size(1Byte)|padding.length(1Byte)|
 codingtable:
 |uChar(1Byte)|codingLength(1Byte)|coding contents|

==Warning==
input file(to compress) MAX size is 512.0 MB


--TO DO--
1. merge C++ so file to python