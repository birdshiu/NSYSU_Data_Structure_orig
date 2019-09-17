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
    int leftSize = middle - front + 1, rightSize = tail - middle;

    int *leftSubArray = new int[leftSize],
        *rightSubArray = new int[rightSize];
    //not sure copy range
    memcpy(leftSubArray, arr + front, leftSize - 1);
    memcpy(rightSubArray, arr + middle + 1, rightSize - 1);

    int i = 0, j = 0, k = front;
    while (i < leftSize && j < rightSize) {
        if (leftSubArray[i] <= rightSubArray[j])
            arr[k] = leftSubArray[i++];
        else
            arr[k] = rightSubArray[j++];
        k++;
    }
    while (i < leftSize) {  //still have element in leftSubArray
        arr[k] = leftSubArray[i++];
        k++;
    }
    while (j < leftSize) {
        arr[k] = rightSubArray[j++];
        k++;
    }
    delete[] leftSubArray;
    delete[] rightSubArray;
}

void mergeSort(int* arr, int front, int tail) {
    if (front < tail) {
        int middle = (front + tail) / 2;
        mergeSort(arr, front, middle);
        mergeSort(arr, middle + 1, tail);
        merge(arr, front, tail, middle);
    }
}

int main() {
    vector<pair<int*, int>> arraySets;
    getRawArray(arraySets);
    cout << "read file done" << endl;
    return 0;
}