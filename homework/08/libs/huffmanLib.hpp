#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <map>
#include <string>
#include <vector>
#define uChar unsigned char

using namespace std;

class HuffmanNode
{
public:
    HuffmanNode(uChar, int);
    HuffmanNode(HuffmanNode *, HuffmanNode *);
    int frequency;
    uChar byteCode;
    std::string decompressCode;
    HuffmanNode *parent, *left, *right;
};
static HuffmanNode *mergeHuffmanTree(map<uChar, int> &nodeTable);
static void assignCompressCode(HuffmanNode *current, string code);
static void recordingLeafs(HuffmanNode *current, vector<HuffmanNode *> &leafs);
static void encoding(vector<HuffmanNode *> &leafs,
                     vector<uChar> &rawData,
                     vector<bool> &encodedData);
static string decoding(map<string, uChar> &_table, string &bitStringData);
static void writeCompressResult(string inputFileName,
                                int originSize,
                                HuffmanNode *root,
                                vector<uChar> &rawData);
bool compress(string fileName);
bool decompress(string fileName);
#endif