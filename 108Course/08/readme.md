Using cpp to speed up the encoding efficient, and actually Cpp is my main programming language

Will remove the huffmanCoding.py
The huffmanCoding.py is just for tersting

--Cpp is awesome--

compile info
// remind me compiling cpp to so(cdll)
g++ -c -fPIC huffmanInCpp.cpp -o tmp.o
g++ -shared tmp.o -o huffman.so
https://stackoverflow.com/questions/145270/calling-c-c-from-python


# Header format 
 little endian
 header format:
 |originSize(4Bytes)|compressBitsLength (4Bytes)|IEEE-754 for compressRate(4Byte)|
 |codingTable.size(1Byte)|padding.length(1Byte)|
 codingtable:
 |uChar(1Byte)|codingLength(1Byte)|coding contents|


--TO DO--
1. fix coded data 
2. coding length must bigger to repersnt larger file
3. more efficiency
    * not use string to decode ?
    * reduce memory useage 