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

auto readOriginFileToVector(string fileName, vector<uChar>& rawData) {
    // will be return the file size by byte
    ifstream inFile(fileName, ios::in | ios::binary);
    if (inFile.fail())
        throw "Read File Error";
    if (inFile.good())
        rawData = vector<uChar>(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
    auto fullSize = rawData.size();
    inFile.close();
    return fullSize;
}

struct cmpNodes {
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) {
        if (lhs->frequency != rhs->frequency)
            return lhs->frequency > rhs->frequency;
        return lhs->byteByAscii >= rhs->byteByAscii;
    }
};

void printAllCompressCode(vector<HuffmanNode*>& records) {
    int sumLength = 0;
    for (auto i : records) {
        cout << (int)i->byteByAscii << " " << i->decompressCode << endl;
        sumLength += i->decompressCode.size();
    }
    cout << "mean length is: " << sumLength / records.size() << endl;
}

pair<uChar*, int> convertToByte(int _value) {
    uChar* arr = new uChar[sizeof(_value)]();
    memcpy(arr, &_value, sizeof(_value));
    return make_pair(arr, sizeof(_value));
}

pair<uChar*, int> convertToByte(float _value) {
    uChar* arr = new uChar[sizeof(_value)]();
    memcpy(arr, &_value, sizeof(_value));
    return make_pair(arr, sizeof(float));
}

pair<uChar*, int> convertToByte(string bitString) {
    int arrSize = bitString.length() / 8;
    uChar* arr = new uChar[arrSize]();
    int counter = 0, bitBuffer = 0;
    for (auto i : bitString) {
        bitBuffer *= 2;
        bitBuffer += (i - '0');

        if (counter % 8 == 7) {
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

int convertToINT(const vector<uChar>& _container) {
    // convert 4 bytes to INT
    int result = 0;
    memcpy(&result, &_container[0], sizeof(int));
    return result;
}

void pushByteToVector(uChar* arr, int arrSize, vector<uChar>& _dest) {
    for (int i = 0; i < arrSize; i++)
        _dest.push_back(arr[i]);
}

int genPaddingLength(int _value) {
    return ((int(_value / 8) + 1) * 8 - _value) % 8;
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

void writeHeader(ofstream& outFile, size_t originSize, size_t compressSize, vector<HuffmanNode*> leafs, size_t encodedDataSize) {
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
    int paddingLength = genPaddingLength(encodedDataSize);
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

string byte2BinaryString(uChar* arr, int arrSize, int paddingLength) {
    string tmp;
    string result;
    for (int i = 0; i < arrSize; i++) {
        uChar oneByteBuffer = 0;
        memcpy(&oneByteBuffer, &arr[i], sizeof(uChar));
        for (int j = 0; j < 8; j++) {
            tmp.push_back(char(abs((oneByteBuffer % 2)) + '0'));  // sometimes there is strange for mod 2 getting -1, hence use adb to make it possitive
            oneByteBuffer /= 2;
        }
        reverse(tmp.begin(), tmp.end());
        result.append(tmp);
        tmp = "";
    }
    result.erase(result.end() - paddingLength, result.end());  //remove padding
    return result;
}

void printReadDecodeTable(map<string, uChar>& decodeTable) {
    for (auto i : decodeTable)
        cout << i.second << " " << i.first << endl;
}

void stepForward(vector<uChar>& _data, int steps) {
    _data.erase(_data.begin(), _data.begin() + steps);
}

auto readHeader(vector<uChar>& rawData) {
    int originSize = 0, compressBitsLength = 0,
        codingTableSize = 0, datapaddingLength = 0;

    originSize = convertToINT({rawData.at(0),
                               rawData.at(1),
                               rawData.at(2),
                               rawData.at(3)});
    stepForward(rawData, sizeof(int));
    compressBitsLength = convertToINT({rawData.at(0),
                                       rawData.at(1),
                                       rawData.at(2),
                                       rawData.at(3)});
    stepForward(rawData, sizeof(int));
    stepForward(rawData, sizeof(float));  //remove compress rate

    codingTableSize = int(rawData.at(0));
    stepForward(rawData, sizeof(char));

    datapaddingLength = int(rawData.at(0));
    stepForward(rawData, sizeof(char));

    return make_tuple(originSize, compressBitsLength, codingTableSize, datapaddingLength);
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
        for (int j = 0; j < totalBytes; j++) {
            codingBuffer[j] = rawData.at(j + 2);
        }
        string code = byte2BinaryString(codingBuffer, totalBytes, paddingLength);
        decodeTable.insert(make_pair(code, element));

        delete[] codingBuffer;
        stepForward(rawData, totalBytes + 2);
    }
    //printReadDecodeTable(decodeTable);
}

string bitStream2String(vector<uChar>& rawData, int paddingLength) {
    string dataBits = byte2BinaryString(&rawData[0], rawData.size(), 0);
    if (paddingLength)
        dataBits.erase(dataBits.end() - paddingLength, dataBits.end());  //remove padding
    return dataBits;
}
}  // namespace tools
#endif