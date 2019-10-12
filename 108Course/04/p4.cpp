#include <cstring>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

class Dish {
   public:
    Dish(int x, int y, int _orientedFrom) {
        this->destination = make_pair(x, y);
        this->orientedFrom = _orientedFrom;
    }
    Dish(pair<int, int> _destination, int _orientedFrom) {
        this->destination = _destination;
        this->orientedFrom = _orientedFrom;
    }
    pair<int, int> destination;
    int orientedFrom;
};

class Stack {
   public:
    Stack();
    void push(const Dish&);
    void pop();
    bool empty();
    int size();
    const Dish top();

   private:
    vector<Dish> _stack;
};

int** makeMap(int n) {
    int** arr = new int*[n];
    for (int i = 0; i < n; i++) {
        arr[i] = new int[n];
        memset(arr[i], 0, sizeof(int) * n);
    }
    return arr;
}

void deleteMap(int** arr, int size) {
    for (int i = 0; i < size; i++)
        delete[] arr[i];
}

void printMap(int** arr, int size) {
    for (int j = 0; j < size; j++) {
        for (int k = 0; k < size; k++)
            cout << arr[j][k] << " ";
        cout << endl;
    }
}

bool canAccess(int** arr, int size, int x, int y) {
    return !(x >= size || x < 0 || y >= size || y < 0 || arr[x][y]);
}

void traversal(int** arr, int size, pair<int, int> current, int steps, Stack& A, int hadView) {
    if (steps == size * size) {
        printMap(arr, size);
        return;
    }
    if (A.empty()) {
        cout << "no solution" << endl;
        return;
    }

    //K1
    if (canAccess(arr, size, current.first - 2, current.second + 1) && hadView < 1) {
        current = make_pair(current.first - 2, current.second + 1);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 1));
        traversal(arr, size, current, steps, A, 0);
    }
    //K2
    else if (canAccess(arr, size, current.first - 1, current.second + 2) && hadView < 2) {
        current = make_pair(current.first - 1, current.second + 2);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 2));
        traversal(arr, size, current, steps, A, 0);
    }
    //K3
    else if (canAccess(arr, size, current.first + 1, current.second + 2) && hadView < 3) {
        current = make_pair(current.first + 1, current.second + 2);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 3));
        traversal(arr, size, current, steps, A, 0);
    }
    //K4
    else if (canAccess(arr, size, current.first + 2, current.second + 1) && hadView < 4) {
        current = make_pair(current.first + 2, current.second + 1);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 4));
        traversal(arr, size, current, steps, A, 0);
    }
    //K5
    else if (canAccess(arr, size, current.first + 2, current.second - 1) && hadView < 5) {
        current = make_pair(current.first + 2, current.second - 1);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 5));
        traversal(arr, size, current, steps, A, 0);
    }
    //K6
    else if (canAccess(arr, size, current.first + 1, current.second - 2) && hadView < 6) {
        current = make_pair(current.first + 1, current.second - 2);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 6));
        traversal(arr, size, current, steps, A, 0);
    }
    //K7
    else if (canAccess(arr, size, current.first - 1, current.second - 2) && hadView < 7) {
        current = make_pair(current.first - 1, current.second - 2);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 7));
        traversal(arr, size, current, steps, A, 0);
    }
    //K8
    else if (canAccess(arr, size, current.first - 2, current.second - 1) && hadView < 8) {
        current = make_pair(current.first - 2, current.second - 1);
        arr[current.first][current.second] = ++steps;
        A.push(Dish(current, 8));
        traversal(arr, size, current, steps, A, 0);
    } else {
        arr[current.first][current.second] = 0;
        hadView = A.top().orientedFrom;
        A.pop();
        current = A.top().destination;
        traversal(arr, size, current, --steps, A, hadView);
    }
}

int main() {
    for (int i = 1; i <= 6; i++) {
        Stack A;
        int** arr = makeMap(i);
        pair<int, int> current{0, 0};
        int steps = 1, hadView;
        bool isBack = false;

        A.push(Dish(current, 0));
        arr[0][0] = steps;
        traversal(arr, i, current, steps, A, 0);
        deleteMap(arr, i);
    }
    return 0;
}

Stack::Stack() {
    this->_stack.clear();
}

void Stack::push(const Dish& _newDish) {
    this->_stack.push_back(_newDish);
}

void Stack::pop() {
    this->_stack.pop_back();
}

bool Stack::empty() {
    return (this->_stack.size() == 0);
}

int Stack::size() {
    return this->_stack.size();
}

const Dish Stack::top() {
    return this->_stack.back();
}