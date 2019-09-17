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

void merge(int* arr, int front, int tail, int middle) {
    int leftSubArray[middle + 1], rightSubArray = [middle + 1];
    // not sure here
    memcpy(leftSubArray, arr + front, middle);
    memcpy(rightSubArray, arr + middle + 1, middle);
    leftSubArray[middle] = INT32_MAX;
    rightSubArray[middle] = INT32_MAX;
}

void mergeSort(int* arr, int front, int tail) {
    if (front < tail) {
        int middle = (front + tail) / 2;
        mergeSort(arr, front, middle);
        mergeSort(arr, middle + 1, tail);
        merge(arr, front, end, middle);
    }
}

int main() {
    vector<pair<int*, int>> arraySets;
    getRawArray(arraySets);
    cout << "read file done" << endl;
    return 0;
}