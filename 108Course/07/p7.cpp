#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

int* generateArray(int size) {
    return new int[size];
}

void getRawArray(vector<pair<int*, int>>& arraySets) {
    ifstream file("input.txt", ifstream::in);
    int n;
    while (file >> n) {
        int* arr = generateArray(n);
        for (int i = 0; i < n; i++)
            file >> arr[i];
        arraySets.push_back(make_pair(arr, n));
    }
    file.close();
}

void insertionSort(int* arr, int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main() {
    vector<pair<int*, int>> arraySets;
    getRawArray(arraySets);
    cout << "read file done" << endl;
    return 0;
}