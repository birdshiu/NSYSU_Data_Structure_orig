#include <cmath>
#include <iostream>
using namespace std;

int main() {
    int num, cases = 1;
    while (cin >> num && num > 0) {
        int answer = 0;
        //cout << num << endl;
        for (; num; num = num >> 1)
            answer++;
        printf("Case %d: %d\n", cases++, answer);
    }
    return 0;
}