#ifndef HUFFMAN
#define HUFFMAN

#include <map>
#include <string>
#include <vector>
#include "huffmanInCpp.hpp"
#define uChar unsigned char

using namespace std;

class HuffmanNode {
   public:
    HuffmanNode(uChar, int);
    HuffmanNode(HuffmanNode*, HuffmanNode*);

    friend bool operator<(const HuffmanNode& lhs, const HuffmanNode& rhs);

    int frequency;
    uChar byteByAscii;
    std::string decompressCode;
    HuffmanNode *parent, *left, *right;
};
HuffmanNode* mergeHuffmanTree(map<uChar, int>& nodeTable);
void assignCompressCode(HuffmanNode* current, string code);
void recordingLeafs(HuffmanNode* current, vector<HuffmanNode*>& leafs);
void encoding(vector<HuffmanNode*>& leafs, vector<uChar>& rawData, vector<bool>& encodedData);
void writeCompressResult(string inputFileName, int originSize, HuffmanNode* root, vector<uChar>& rawData);
void printAllCompressCode(vector<HuffmanNode*>& records);
bool compress(string fileName);
bool decompress(string fileName);
#endif