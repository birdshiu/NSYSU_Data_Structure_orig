#ifndef HUFFMAN
#define HUFFMAN

#include <map>
#include <vector>
#include "huffmanInCpp.hpp"
#define uChar unsigned char

using namespace std;

class HuffmanNode {
   public:
    HuffmanNode(uChar, int);
    HuffmanNode(HuffmanNode*, HuffmanNode*);

    friend bool operator<(const HuffmanNode& lhs, const HuffmanNode& rhs);

    int frequency, codingLength;
    uChar byteByAscii;
    long decompressCode;
    HuffmanNode *parent, *left, *right;
};

#endif