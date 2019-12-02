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
    regex getFileName("[^/]*$");
    string outputName;
    regex_search(inputFileName, outputName, getFileName);
    outputName.append(".compress");
    return outputName;
}

struct cmpNodes {
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) {
        if (lhs->frequency != rhs->frequency)
            return lhs->frequency < rhs->frequency;
        return lhs->byteByAscii > rhs->byteByAscii;
    }
};

//concatenate function
vector<uChar> concatenateBits(vector<uChar> resultOfConcatenate,
                              vector<uChar> originData,
                              int bitsWidth) {
    
}
}  // namespace tools