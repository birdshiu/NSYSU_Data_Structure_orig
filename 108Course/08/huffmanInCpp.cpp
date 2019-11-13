#include "huffmanInCpp.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>
#define uChar unsigned char
using namespace std;

struct cmp {
    bool operator()(const HuffmanNode& lhs, const HuffmanNode& rhs) {
        if (lhs.frequency != rhs.frequency)
            return lhs.frequency < rhs.frequency;
        return lhs.codingLength > rhs.codingLength;
    }
};

HuffmanNode::HuffmanNode(uChar _singleByte, int _frequency) {
    byteByAscii = _singleByte;
    frequency = _frequency;
    codingLength = 0;
    decompressCode = 0;
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
        rawData = vector<uChar> v(istreambuf_iterator<uChar>{inFile}, {});
    int fullSize = inFile.tellg();
    inFile.close();
    return fullSize;
}

HuffmanNode* mergeHuffmanTree(map<uChar, int>& nodeTable) {
    priority_queue<HuffmanNode, vector<HuffmanNode>, cmp> pq;
    for (auto i : nodeTable) {
        auto newNode = new HuffmanNode(i.first, i.second);
        pq.push(newNode);
    }
    while (pq.size() > 1) {
        auto a = pq.top();
        pq.pop();
        auto b = pq.top();
        pq.pop();

        auto mergedNode = new HuffmanNode(&a, &b);
        pq.push(mergedNode);
    }
    return &(pq.top());
}

void assignCompressCode(HuffmanNode* current, string code = "") {
    if (current->left == nullptr && current->right == nullptr)  //isLeaf
        current->decompressCode = code;
    if (current->left)
        assignCompressCode(current->left, code.append("0"));
    if (current->right)
        assignCompressCode(current->right, code.append("1"));
}

void writeCompressResult(int originSize) {
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
}

void decompress(string fileName) {
}
