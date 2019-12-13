huffman coding
===
This is a huffman encoding file application.<br>
You can use huffman.elf or main.cpp singly. <br>
if you wnat to use this in debian environment, suggest you run setup.sh first!<br>

# How to build
* Compile cpp/hpp to elf:
    * g++ -std=c++17 main.cpp tools.hpp huffmanLib.cpp huffmanLib.hpp -o huffman.elf
* Compile to so file for dynamic linking in python:
    * g++-9 -fPIC -rdynamic -shared -std=c++2a tools.hpp huffmanLib.cpp huffmanLib.hpp -o huffman.so
    * ref: https://stackoverflow.com/questions/145270/calling-c-c-from-python
* setup python environment
    * ./setup.sh

# How to use
* python(GUI):
    * python3 p8.py
* elf:
    * ./huffman.elf ${file} compress
    * ./huffman.elf ${file} decompress

# Header format 
 * little endian
 * header format(2 part):<br>
    info:<br>
        <pre><code>|originSize(sizeof(size_t))|compressBitsLength (sizeof(size_t))|IEEE-754 for compressRate(4Byte)|</code></pre><br>
        <pre><code>|codingTable.size(1Byte)|padding.length(1Byte)|</code></pre><br>
    codingtable:<br>
        <pre><code>|uChar(1Byte)|codingLength(1Byte)|coding contents|</code></pre><br>

==Warning==<br>
input file(to compress) MAX size is 2^31 GB<br>