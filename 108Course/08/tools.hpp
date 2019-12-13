#ifndef TOOLS
#define TOOLS
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "huffmanLib.hpp"
namespace tools {
using namespace std;

bool readOriginFileToVector(string fileName, vector<uChar>& rawData) {
    //return is file good?
    ifstream inFile(fileName, ios::in | ios::binary);
    if (inFile.fail())
        throw "Read File Error";
    if (inFile.good())
        rawData = vector<uChar>(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
    inFile.close();
    return inFile.good();
}

struct cmpNodes {
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) {
        if (lhs->frequency != rhs->frequency)
            return lhs->frequency > rhs->frequency;
        return lhs->byteByAscii >= rhs->byteByAscii;
    }
};

void pushByteToVector(uChar* arr, int arrSize, vector<uChar>& _dest) {
    for (int i = 0; i < arrSize; i++)
        _dest.push_back(arr[i]);
}

int genPaddingLength(int _value) {
    return ((int(_value / 8) + 1) * 8 - _value) % 8;
}

void stepForward(vector<uChar>& _data, int steps) {
    _data.erase(_data.begin(), _data.begin() + steps);
}

void printCodingTable(vector<HuffmanNode*>& records) {
    for (auto i : records)
        cout << (int)i->byteByAscii << " " << i->decompressCode << endl;
}

void printCodingTable(map<string, uChar>& decodeTable) {
    for (auto i : decodeTable)
        cout << i.second << " " << i.first << endl;
}

void viewByteAsINT(vector<uChar>& bytesArray, int elementsPerLine = 20) {
    unsigned int counter = 0;
    for (auto i : bytesArray) {
        cout << int(i) << "|\t";
        if ((++counter) % elementsPerLine == 0)
            cout << "\n";
    }
    cout << endl;
}

pair<uChar*, int> convertToByte(size_t _value) {
    uChar* arr = new uChar[sizeof(_value)]();
    memcpy(arr, &_value, sizeof(_value));
    return make_pair(arr, sizeof(_value));
}

pair<uChar*, int> convertToByte(float _value) {
    uChar* arr = new uChar[sizeof(_value)]();
    memcpy(arr, &_value, sizeof(_value));
    return make_pair(arr, sizeof(float));
}

template <typename Iterable>
pair<uChar*, int> convertToByte(const Iterable& data) {
    int arrSize = data.size() / 8;
    uChar* arr = new uChar[arrSize]();
    unsigned int counter = 0, bitBuffer = 0;
    for (auto i : data) {
        bitBuffer <<= 1;
        bitBuffer += (i + '0') % 2;

        if ((counter % 8 == 7) && counter) {
            arr[(counter) / 8] = char(bitBuffer);
            bitBuffer = 0;
        }
        counter++;
    }
    return make_pair(arr, arrSize);
}

size_t convertToSize_t(const vector<uChar>& _container) {
    size_t result = 0;
    memcpy(&result, &_container[0], sizeof(size_t));
    return result;
}

string convertToBitString(uChar* arr, int arrSize, int paddingLength) {
    string tmp;
    string result;
    for (int i = 0; i < arrSize; i++) {
        uChar oneByteBuffer = 0;
        memcpy(&oneByteBuffer, &arr[i], sizeof(uChar));
        for (int j = 0; j < 8; j++) {
            tmp.push_back(char(abs((oneByteBuffer % 2)) + '0'));
            oneByteBuffer /= 2;
        }
        reverse(tmp.begin(), tmp.end());
        result.append(tmp);
        tmp = "";
    }
    result.erase(result.end() - paddingLength, result.end());  //remove padding
    return result;
}

string convertToBitString(vector<uChar>& rawData, int paddingLength) {
    return convertToBitString(&rawData[0], rawData.size(), paddingLength);
}

void writeHeader(ofstream& outFile, size_t originSize, size_t compressSize, vector<HuffmanNode*> leafs) {
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
    //here need put padding length
    int paddingLength = genPaddingLength(compressSize);
    headerBytes.push_back(char(paddingLength));

    for (auto i : leafs) {
        headerBytes.push_back(i->byteByAscii);                          //uChar
        headerBytes.push_back(char(i->decompressCode.length() % 256));  //coding length, means bits
        paddingLength = genPaddingLength(i->decompressCode.length());
        auto code = i->decompressCode;
        for (int j = 0; j < paddingLength; j++)
            code.append("0");  // padding coding contents

        auto [encodeArray, encodeArraySize] = convertToByte(code);  // divie after padding string to uChar
        pushByteToVector(encodeArray, encodeArraySize, headerBytes);
        delete[] encodeArray;
    }

    //viewByteAsINT(headerBytes);
    for (auto i : headerBytes)
        outFile << i;
}

auto readHeader(vector<uChar>& rawData) {
    size_t originSize = 0, compressBitsLength = 0;
    int codingTableSize = 0, dataPaddingLength = 0;

    originSize = convertToSize_t(vector<uChar>(rawData.begin(), rawData.begin() + sizeof(size_t)));
    stepForward(rawData, sizeof(size_t));
    compressBitsLength = convertToSize_t(vector<uChar>(rawData.begin(), rawData.begin() + sizeof(size_t)));
    stepForward(rawData, sizeof(size_t));
    stepForward(rawData, sizeof(float));  //remove compress rate

    codingTableSize = int(rawData.at(0));
    stepForward(rawData, sizeof(char));
    dataPaddingLength = int(rawData.at(0));
    stepForward(rawData, sizeof(char));

    return make_tuple(originSize, compressBitsLength, codingTableSize, dataPaddingLength);
}

void readDecodeTable(vector<uChar>& rawData, map<string, uChar>& decodeTable, int decodingTableSize) {
    if (decodingTableSize == 0)
        decodingTableSize = 256;
    for (int i = 0; i < decodingTableSize; i++) {
        uChar element = rawData.at(0);
        int codingLength = int(rawData.at(1));  // length of bits
        int paddingLength = genPaddingLength(codingLength);
        int totalBytes = (codingLength + paddingLength) / 8;

        uChar* codingBuffer = new uChar[totalBytes]();
        for (int j = 0; j < totalBytes; j++)
            codingBuffer[j] = rawData.at(j + 2);
        string code = convertToBitString(codingBuffer, totalBytes, paddingLength);
        decodeTable.insert(make_pair(code, element));

        delete[] codingBuffer;
        stepForward(rawData, totalBytes + 2);
    }
    //printCodingTable(decodeTable);
}

}  // namespace tools
#endif