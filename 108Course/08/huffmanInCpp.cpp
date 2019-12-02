#include "huffmanInCpp.hpp"
#include <algorithm>
#include <bitset>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <vector>
#include "tools.hpp"
#define uChar unsigned char
using namespace std;

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
    bool isSumByteOverflow = (left->byteByAscii + right->byteByAscii) > 255;
    byteByAscii = (isSumByteOverflow) ? 255 : left->byteByAscii + right->byteByAscii;
    //maybe not done
}

HuffmanNode* mergeHuffmanTree(map<uChar, int>& nodeTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, tools::cmpNodes> pq;
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

void assignCompressCode(HuffmanNode* current, string code) {
    if (current->left)
        assignCompressCode(current->left, code + "0");
    if (current->left == nullptr && current->right == nullptr)
        current->decompressCode = code;
    if (current->right)
        assignCompressCode(current->right, code + "1");
}

void recordingLeafs(HuffmanNode* current, vector<HuffmanNode*>& leafs) {
    if (current->left)
        recordingLeafs(current->left, leafs);
    if (current->left == nullptr && current->right == nullptr)
        leafs.push_back(current);
    if (current->right)
        recordingLeafs(current->right, leafs);
}

void encoding(vector<HuffmanNode*>& leafs, vector<uChar>& rawData, vector<bool>& encodedData) {
    map<uChar, string> encodingTable;
    for (auto i : leafs)
        encodingTable[i->byteByAscii] = i->decompressCode;
    for (auto iter = rawData.begin(); iter != rawData.end(); iter++) {
        auto getCode = find(encodingTable.begin(), encodingTable.end(), *iter);
        if (getCode == encodingTable.end())
            throw "no match on encoding table!";

        //fill bits into encodedData
        auto bitsString = getCode->second;
        for (auto i : bitsString)
            encodedData.push_back(i - '0');
    }
}

void writeCompressResult(string inputFileName, int originSize, HuffmanNode* root, vector<uChar>& rawData) {
    vector<HuffmanNode*> leafs;
    vector<bool> encodedData;
    string outputName = tools::genOutputName(inputFileName);
    ofstream outFile(outputName);

    recordingLeafs(root, leafs);
    try {
        encoding(leafs, rawData, encodedData);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    auto CompressedSize = encodedData.size() * sizeof(bool);

    /*outFile << "Origin file size(Byte): " << originSize << endl;
    outFile << "Comperssed file size(Byte): " << CompressedSize << endl;
    outFile << "Compress rate: " << CompressedSize * 1.0 / originSize << endl;
    

    for (auto nodes : leafs) {
        outFile << nodes->byteByAscii << " = " << nodes->decompressCode << endl;
    }*/

    outFile << "----------" << endl;
    // output compressed Data
    /*for (auto i : rawData) {
        bitset<bitsWidth> b(i);  //must be const, hence will use concatenate bits to output
        outFile << b;
    }*/

    outFile.close();
}

void compress(string fileName) {
    vector<uChar> rawData;
    int inputSize;
    HuffmanNode* root;

    //read raw file to vector
    try {
        inputSize = tools::readOriginFileToVector(fileName, rawData);
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
    assignCompressCode(root, "");
    vector<HuffmanNode*> leafs;
    recordingLeafs(root, leafs);
    tools::printAllCompressCode(leafs);
    //writeCompressResult(fileName, inputSize, root, rawData);
}

void decompress(string fileName) {
}

/*
extern "C"{
   HuffmanNode obj;
   
}*/