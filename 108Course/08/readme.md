huffman coding
===
This is a huffman encoding file application.<br>
You can use huffman.elf or main.cpp singly. <br>
if you wnat to use this in debian environment, suggest you run setup.sh first!<br>

# How to build
* Compile cpp/hpp to elf:
    * g++ -std=c++17 main.cpp libs/tools.hpp libs/huffmanLib.cpp libs/huffmanLib.hpp -o huffman.elf
* Compile to so file for dynamic linking in python:
    * g++-9 -fPIC -rdynamic -shared -std=c++2a libs/tools.hpp libs/huffmanLib.cpp libs/huffmanLib.hpp -o libs/huffman.so
    * ref: https://stackoverflow.com/questions/145270/calling-c-c-from-python
* setup python environment
    * ./setup.sh

# How to use
* python(GUI):
    * python3 p8.py
* zenity(GUI):
    1. <pre><code>cd zenity/</code></pre>
    2. <pre><code>./shellGUI.sh</code></pre>
* elf:
    * <pre><code>./huffman.elf ${file} -c</code></pre>
    * <pre><code>./huffman.elf ${file} -d</code></pre>

# Header format 
 * little endian
 * header format(2 part):<br>
    info:<br>
        <pre><code>|originSize(sizeof(size_t))|compressBitsLength (sizeof(size_t))|IEEE-754 for compressRate(4Byte)|<br>
        |codingTable.size(1Byte)|padding.length(1Byte)|</code></pre>
    codingtable:<br>
        <pre><code>|uChar(1Byte)|codingLength(1Byte)|coding contents|</code></pre><br>

==Warning==<br>
input file(to compress) MAX size is 2^31 GB<br>

# Might do
1. more clear and efficient while compress
    * assign code have to traversal once
    * record leafs also traversal once
    * after recording leafs push all leafs in to map?
    * **why don't I just push code into map once**
        * Will this be "not clean code"?