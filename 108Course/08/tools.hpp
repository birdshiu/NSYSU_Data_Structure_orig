#ifndef TOOLS
#define TOOLS
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "huffmanInCpp.hpp"
namespace tools {
using namespace std;

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

int getNumberWidth(uChar a) {
    int counter = 0;
    while (a) {
        counter++;
        a /= 10;
    }
    return counter;
}

string genOutputName(string inputFileName) {
    inputFileName.append(".compress");
    return inputFileName;
}

struct cmpNodes {
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) {
        if (lhs->frequency != rhs->frequency)
            return lhs->frequency > rhs->frequency;
        return lhs->byteByAscii < rhs->byteByAscii;
    }
};

void printAllCompressCode(vector<HuffmanNode*>& records) {
    int sumLength = 0;
    for (auto i : records) {
        cout << i->byteByAscii << " " << i->decompressCode << endl;
        sumLength += i->decompressCode.size();
    }
    cout << "mean length is: " << sumLength / records.size() << endl;
}

pair<uChar*, int> convertToByte(int _value) {
    uChar* arr = new uChar[sizeof(_value)]();
    unsigned char const* p = reinterpret_cast<unsigned char const*>(&_value);
    memcpy(arr, p, sizeof(int));
    return make_pair(arr, sizeof(int));
}

pair<uChar*, int> convertToByte(float _value) {
    uChar* arr = new uChar[sizeof(_value)]();
    unsigned char const* p = reinterpret_cast<unsigned char const*>(&_value);
    memcpy(arr, p, sizeof(float));
    return make_pair(arr, sizeof(float));
}

pair<uChar*, int> convertToByte(string bitString) {
    int arrSize = bitString.length() / 8;
    uChar* arr = new uChar[arrSize]();
    int counter = 0, bitBuffer = 0;
    for (auto i : bitString) {
        bitBuffer *= 2;
        bitBuffer += (i - '0');

        if ((counter % 8 == 7) && counter) {
            arr[(counter) / 8] = char(bitBuffer);
            bitBuffer = 0;
        }
        counter++;
    }

    return make_pair(arr, arrSize);
}

pair<uChar*, int> convertToByte(vector<bool>& encodedData) {
    int arrSize = encodedData.size() / 8;
    uChar* arr = new uChar[arrSize]();
    int counter = 0, bitBuffer = 0;
    for (auto i : encodedData) {
        bitBuffer *= 2;
        bitBuffer += int(i);

        if ((counter % 8 == 7) && counter) {
            arr[(counter) / 8] = char(bitBuffer);
            bitBuffer = 0;
        }
        counter++;
    }
    return make_pair(arr, arrSize);
}

void pushByteToVector(uChar* arr, int arrSize, vector<uChar>& _dest) {
    for (int i = 0; i < arrSize; i++)
        _dest.push_back(arr[i]);
}

int genPeddingLength(int _value) {
    int peddingLength = 8 - (_value % 8);
    if (peddingLength == 8)
        peddingLength = 0;
    return peddingLength;
}

void viewByteAsINT(vector<uChar>& bytesArray) {
    unsigned int counter = 0;
    for (auto i : bytesArray) {
        cout << int(i) << "|\t";
        if ((++counter) % 20 == 0)
            cout << "\n";
    }
    cout << endl;
}

void outputComperssHeader(ofstream& outFile, int originSize, int compressSize, vector<HuffmanNode*> leafs, int encodedDataSize) {
    /*
    * little endian
    * header format:
    * |originSize(4Bytes)|compressSize (4Bytes)|IEEE-754 for compressRate(4Byte)|
    * |codingTable.size(1Byte)|pedding.size(1Byte)|
    * codingtable:
    * |uChar(1Byte)|codingLength(1Byte)|peddingLength(1Byte)|coding contents|
    * 
    */
    float compressRate = compressSize * 1.0 / originSize;
    vector<uChar> headerBytes;

    auto [origin, originArraySize] = convertToByte(originSize);
    pushByteToVector(origin, originArraySize, headerBytes);
    delete[] origin;
    auto [compressed, compressedArraySize] = convertToByte(compressSize);
    pushByteToVector(compressed, compressedArraySize, headerBytes);
    delete[] compressed;
    auto [rate, rateArraySize] = convertToByte(compressRate);
    pushByteToVector(rate, rateArraySize, headerBytes);
    delete[] rate;

    //push codingTable.size()
    headerBytes.push_back(char(leafs.size() % 256));
    //here need put pedding length
    int peddingLength = genPeddingLength(encodedDataSize);
    headerBytes.push_back(char(peddingLength));

    for (auto i : leafs) {
        headerBytes.push_back(i->byteByAscii);                          //uChar
        headerBytes.push_back(char(i->decompressCode.length() % 256));  //coding length, means bits

        peddingLength = genPeddingLength(i->decompressCode.length());
        auto code = i->decompressCode;
        for (int j = 0; j < peddingLength; j++)
            code.append("0");  // pedding coding contents

        auto [encodeArray, encodeArraySize] = convertToByte(code);  // divie after pedding string to uChar
        pushByteToVector(encodeArray, encodeArraySize, headerBytes);
        delete[] encodeArray;
    }

    //viewByteAsINT(headerBytes);

    for (auto i : headerBytes)
        outFile << i;
}
}  // namespace tools
#endif