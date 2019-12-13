#include <iostream>
#include "huffmanLib.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    bool result = false;
    if (argc == 2) {
        if (string(argv[1]) == string("--help") || string(argv[1]) == string("-h")) {
            cout << "[0] ./a.out" << endl
                 << "[1] input filename" << endl
                 << "[2] -c --compress\t to compress the input file" << endl
                 << "    -d --decompress\t to decompress the input file" << endl
                 << "[3] output file name(default is append ~[2] option)" << endl;
            return 0;
        } else {
            cout << "too less argument" << endl;
        }
    } else if (argc == 3) {
        if (string(argv[2]) == string("--compress") || string(argv[2]) == string("-c")) {
            result = compress(string(argv[1]));
        } else if (string(argv[2]) == string("--decompress") || string(argv[2]) == string("-d")) {
            result = decompress(string(argv[1]));
        } else {
            cout << "Wrong option!" << endl;
        }
    } else if (argc < 2) {
        cout << "too less argument" << endl;
    } else {
        cout << "too much arguments" << endl;
    }

    cout << ((result) ? "Success!" : "Failed!") << endl;

    return 0;
}