#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class HuffmanNode {
   public:
    HuffmanNode() {
        memset(this, 0, sizeof(HuffmanNode));
    }
    int frequency, codingLength;
    char byteByAscii;
    long decompressCode;
    HuffmanNode *parent, *left, &right;
};

HuffmanNode* pop(HuffmanNode* allNodes[], int index, bool isRightSide = false) {
    auto current = allNodes[index];
    current->decompressCode = isRightSide;
    current->codingLength = 1;
    return current;
}

void settingNodeCode(HuffmanNode* current) {
    auto thisParent = current->parent;
    while (thisParent && thisParent->codingLength) {
        current->decompressCode <<= 1;
        current->decompressCode |= thisParent->decompressCode;
        current->codingLength++;
        thisParent = thisParent->parent;
    }
}

int generateTree(HuffmanNode fullTree[], bool setCoding) {
    HuffmanNode *allNodes[256], *current = nullptr;
    int counter = 0;
    for (int i = 0; i < 256 && fullTree[i].frequency; i++)
        allNodes[counter++] = &fullTree[i];
    int parentNode = counter, stillInQueue = counter - 1;

    while (stillInQueue > 0) {
        current = &fullTree[parentNode++];
        current->left = pop(allNodes, stillInQueue--);
        current->right = pop(allNodes, stillInQueue--, true);
        current->left->parent = current->right->parent = current;
        current->frequency = current->left->frequency + current->right->frequency;

        int i = stillInQueue;
        for (; i >= 0; i--) {
            if (allNodes[i]->frequency >= current->frequency) {
                for (; i >= 0; i--)
                    if (allNodes[i]->frequency == current->frequency && allNodes[i]->byteByAscii < current->byteByAscii)
                        allNodes[i + 1] = allNodes[i];
                    else
                        break;
                break;
            }
            allNodes[i + 1] = allNodes[i];
        }
        allNodes[i + 1] = current;
        stillInQueue++;
    }
    if (setCoding)
        for (int i = 0; i < counter; i++)
            settingNodeCode(&fullTree[i]);
    return counter;
}

void compress(){

}

void decompress(){
    
}