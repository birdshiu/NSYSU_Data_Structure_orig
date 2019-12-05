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
    //int fullSize = rawData.size();
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
    // convert first 4 btye to INT
    if (_container.size() < 4) {
        throw "wrong size of int";
    }
    uChar buffer[4] = {0};
    for (int i = 0; i < 4; i++)
        buffer[i] = _container.at(i);
    int returnValue = 0;
    memcpy(&returnValue, (char*)buffer, sizeof(int));
    return returnValue;
}

void pushByteToVector(uChar* arr, int arrSize, vector<uChar>& _dest) {
    for (int i = 0; i < arrSize; i++)
        _dest.push_back(arr[i]);
}

int genPaddingLength(int _value) {
    int PaddingLength = 8 - (_value % 8);
    if (PaddingLength == 8)
        PaddingLength = 0;
    return PaddingLength;
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

void writeHeader(ofstream& outFile, int originSize, int compressSize, vector<HuffmanNode*> leafs, int encodedDataSize) {
    /*
    * little endian
    * header format:
    * |originSize(4Bytes)|compressBitsLength (4Bytes)|IEEE-754 for compressRate(4Byte)|
    * |codingTable.size(1Byte)|padding.length(1Byte)|
    * codingtable:
    * |uChar(1Byte)|codingLength(1Byte)|coding contents|
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

void bytes2BitsVector(uChar* arr, int arrSize, int peddingSize, vector<bool>& _destination) {
    unsigned int buffer = 0;
    memcpy(&buffer, arr, arrSize);
    buffer >>= peddingSize;
    for (int i = 0; i < arrSize * 8 - peddingSize; i++) {
        bool tailBit = buffer % 2;
        _destination.push_back(tailBit);
        buffer >>= 1;
    }
    reverse(_destination.begin(), _destination.end());
}

void printReadDecodeTable(map<uChar, vector<bool>>& decodeTable) {
    for (auto i : decodeTable) {
        cout << (int)i.first << " ";
        for (auto j : i.second)
            cout << j;
        cout << endl;
    }
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

void readDecodeTable(vector<uChar>& rawData, map<uChar, vector<bool>>& decodeTable, int decodingTableSize) {
    for (int i = 0; i < decodingTableSize; i++) {
        uChar element = rawData.at(0);
        int codingLength = int(rawData.at(1));  // length of bits
        int paddingLength = ((int(codingLength / 8) + 1) * 8 - codingLength) % 8;

        int totalBytes = (codingLength + paddingLength) / 8;
        decodeTable.insert(make_pair(element, vector<bool>());

        uChar* codingBuffer = new uChar[totalBytes]();
        for (int j = 0; j < totalBytes; j++) {
            codingBuffer[j] = rawData.at(j + 2);
        }
        bytes2BitsVector(codingBuffer, totalBytes, paddingLength, decodeTable.at(element));
        delete[] codingBuffer;
        stepForward(rawData, totalBytes + 2);
    }
    //printReadDecodeTable(decodeTable);
}
}  // namespace tools
#endif