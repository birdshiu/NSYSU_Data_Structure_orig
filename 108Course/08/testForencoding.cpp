#include "huffmanInCpp.hpp"
using namespace std;

int main() {
    compress("dataSet/text");
    //compress("dataSet/random");
    //compress("dataSet/fullAs");
    //decompress("dataSet/fullAs.compress");
    decompress("dataSet/text.compress");
    return 0;
}