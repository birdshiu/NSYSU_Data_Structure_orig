#ifndef HUFFMAN
#define HUFFMAN

#include <map>
#include <string>
#include <vector>
#include "huffmanLib.hpp"
#define uChar unsigned char

using namespace std;

class HuffmanNode {
   public:
    HuffmanNode(uChar, int);
    HuffmanNode(HuffmanNode*, HuffmanNode*);
    int frequency;
    uChar byteByAscii;
    std::string decompressCode;
    HuffmanNode *parent, *left, *right;
};
HuffmanNode* mergeHuffmanTree(map<uChar, int>& nodeTable);
void assignCompressCode(HuffmanNode* current, string code);
void recordingLeafs(HuffmanNode* current, vector<HuffmanNode*>& leafs);
void encoding(vector<HuffmanNode*>& leafs, vector<uChar>& rawData, vector<bool>& encodedData);
string decoding(map<string, uChar>& _table, string& bitStringData);
void writeCompressResult(string inputFileName, int originSize, HuffmanNode* root, vector<uChar>& rawData);
bool compress(string fileName);
bool decompress(string fileName);
#endif