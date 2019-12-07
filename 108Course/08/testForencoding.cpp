#include <iostream>
#include "huffmanInCpp.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    /*
    [0] ./a.out
    [1] input filename 
    [2] compress | decompress
    */
    bool result = false;
    if (argc == 2) {
        if (argv[1] == "--help" || argv[1] == "-h") {
            cout << "[0] ./a.out" << endl
                 << "[1] input filename" << endl
                 << "[2] compress |decompress" << endl
                 << "[3] output file name(default is append ~[2] option)" << endl;
        } else {
            cout << "too less argument" << endl;
        }
    } else if (argc == 3) {
        if (string(argv[2]) == string("compress")) {
            result = compress(string(argv[1]));
        } else if (string(argv[2]) == string("decompress")) {
            result = compress(string(argv[1]));
        } else {
            cout << "Wrong option!" << endl;
        }
    } else if (argc < 2) {
        cout << "too less argument" << endl;
    } else {
        cout << "too much arguments" << endl;
    }
    if (result)
        cout << "Success!" << endl;
    else
        cout << "Failed!" << endl;
    return 0;
}