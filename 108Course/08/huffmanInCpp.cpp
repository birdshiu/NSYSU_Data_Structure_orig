#include "huffmanInCpp.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <vector>
#define uChar unsigned char
using namespace std;

struct cmp {
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) {
        if (lhs->frequency != rhs->frequency)
            return lhs->frequency < rhs->frequency;
        return lhs->byteByAscii > rhs->byteByAscii;
    }
};

HuffmanNode::HuffmanNode(uChar _singleByte, int _frequency) {
    byteByAscii = _singleByte;
    frequency = _frequency;
    codingLength = 0;
    parent = left = right = nullptr;
}

HuffmanNode::HuffmanNode(HuffmanNode* leftNode, HuffmanNode* rightNode) {
    frequency = leftNode->frequency + rightNode->frequency;
    leftNode->parent = rightNode->parent = this;
    left = leftNode;
    right = rightNode;
    parent = nullptr;
    //maybe not done
}

int readOriginFileToVector(string fileName, vector<uChar>& rawData) {
    // will be return the file size by byte
    ifstream inFile(fileName, ios::in | ios::binary);
    if (inFile.fail())
        throw "Read File Error";
    if (inFile.good())
        rawData = vector<uChar>(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
    int fullSize = inFile.tellg();
    inFile.close();
    return fullSize;
}

HuffmanNode* mergeHuffmanTree(map<uChar, int>& nodeTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, cmp> pq;
    for (auto i : nodeTable) {
        auto newNode = new HuffmanNode(i.first, i.second);
        pq.push(newNode);
    }
    while (pq.size() > 1) {
        auto a = pq.top();
        pq.pop();
        auto b = pq.top();
        pq.pop();

        auto mergedNode = new HuffmanNode(a, b);
        pq.push(mergedNode);
    }

    return pq.top();
}

void assignCompressCode(HuffmanNode* current, string code = "") {
    if (current->left)
        assignCompressCode(current->left, code.append("0"));
    if (current->left == nullptr && current->right == nullptr)  //isLeaf
        current->decompressCode = code;
    if (current->right)
        assignCompressCode(current->right, code.append("1"));
}

void recordingLeafs(HuffmanNode* current, vector<HuffmanNode*>& leafs) {
    if (current->left)
        recordingLeafs(current->left, leafs);
    leafs.push_back(current);
    if (current->right)
        recordingLeafs(current->right, leafs);
}

void writeCompressResult(string inputFileName, int originSize, HuffmanNode* root) {
    regex getFileName("[^/]*$");
    string outputName;
    regex_search(inputFileName, outputName, getFileName);
    outputName.append(".compress");

    ofstream outFile(outputName);
    outFile << "Origin file size(Byte): " << originSize << endl;
    //outFile << "Comperssed file size(Byte): " << CompressedSize << endl;
    //outFile << "Compress rate: " << CompressedSize * 1.0 / originSize << endl;

    //encoding table
    vector<HuffmanNode*>leafs;
    recordingLeafs(root, leafs);
    for (auto nodes : leafs)
        outFile << nodes->byteByAscii << "=" << nodes->decompressCode << endl;

    //compress origin to following result

    outFile.close();
}

void compress(string fileName) {
    vector<uChar> rawData;
    int inputSize;
    HuffmanNode* root;

    //read raw file to vector
    try {
        inputSize = readOriginFileToVector(fileName, rawData);
    } catch (const exception& e) {
        cerr << e.what() << '\n';
        exit(1);
    }

    // collect byte frequency in map
    map<uChar, int> nodeTable;  //unsigned char, frequency
    for (auto singleByte : rawData) {
        nodeTable[singleByte]++;
    }

    root = mergeHuffmanTree(nodeTable);
    assignCompressCode(root);
    writeCompressResult(fileName, inputSize, root);
}

void decompress(string fileName) {
}

/*
extern "C"{
   HuffmanNode obj;
   
}*/