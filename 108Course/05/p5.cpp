#include <iostream>
#include <utility>
using namespace std;

struct Term {
    int coef, exp;
    Term* next;
    Term(const int _coef, const int _exp, Term* _next) {
        this->coef = _coef;
        this->exp = _exp;
        this->next = _next;
    }
};

class Chain {
   public:
    Chain() { this->head = nullptr; }
    pair<Term*, bool> findExp(int _exp);
    void addNode(int _coef, int _exp);
    void cleanNode();
    void freeChain();

   protected:
    Term* head = nullptr;
};

class Poly : public Chain {
   public:
    Poly() { Chain(); }
    Poly operator+(const Poly);
    Poly operator*(const Poly);
    friend ostream& operator<<(ostream&, const Poly&);
};

int main() {
    int p, q, cases = 1, coef, exp;
    Poly A, B, C, D;
    while (1) {
        A.freeChain();
        B.freeChain();
        C.freeChain();
        D.freeChain();

        cin >> p;
        for (int i = 0; i < p; i++) {
            cin >> coef >> exp;
            A.addNode(coef, exp);
        }
        cin >> q;
        for (int i = 0; i < q; i++) {
            cin >> coef >> exp;
            B.addNode(coef, exp);
        }
        if (p == 0 && q == 0)
            break;

        C = A + B;
        D = A * B;

        printf("Case%d:\n", cases++);
        cout << "ADD\n"
             << C << "MULTIPLY\n"
             << D;
    }
    return 0;
}

pair<Term*, bool> Chain::findExp(int _exp) {
    /*
    * the return value will be a pair like (pointer, isExpSame)
    * the first arg which "pointer's next is the palce you will insert"
    * the second arg is true if _exp == pointer->exp
    */
    Term *current = this->head, *preverious = this->head;
    while (current && current->exp > _exp) {
        preverious = current;
        current = current->next;
    }
    if (current && current->exp == _exp)
        return make_pair(current, true);
    return make_pair(preverious, false);
}

void Chain::addNode(int _coef, int _exp) {  // need sort first
    Term* newTerm = new Term(_coef, _exp, nullptr);
    if (this->head) {                                   // if chain is not empty
        pair<Term*, bool> insertPlace = findExp(_exp);  // find where can I insert new Term in

        if (insertPlace.second) {
            insertPlace.first->coef += _coef;  // has the same exp just add the coef
        } else if (insertPlace.first == this->head && insertPlace.first->exp < _exp) {
            newTerm->next = this->head;
            this->head = newTerm;  // is the head of the Chain aka newTerm->exp is greater than the head
        } else if (insertPlace.first == this->head && insertPlace.first->exp > _exp) {
            newTerm->next = this->head->next;
            this->head->next = newTerm;
        } else if (insertPlace.first->next) {
            newTerm->next = insertPlace.first->next;
            insertPlace.first->next = newTerm;  // the middle of the Chain
        } else {
            insertPlace.first->next = newTerm;  // the tail
        }
    } else {
        this->head = newTerm;
    }
}

void Chain::cleanNode() {
    auto preverious = this->head;
    for (auto current = this->head; current; current = current->next) {
        if (current->coef) {  // if the coefficeint is not zero
            preverious = current;
        } else {
            preverious->next = current->next;
        }
    }
    while (this->head && this->head->coef == 0) {  // boundary condition
        this->head = this->head->next;
    }
}

void Chain::freeChain() {
    auto temp = this->head;
    for (auto i = this->head; i != nullptr; i = temp) {
        temp = i->next;
        delete (i);
    }
    this->head = nullptr;  //after delete need to reset the head, or it will be dangling pointer
}

Poly Poly::operator+(const Poly source) {
    Poly* result = new Poly();
    for (auto i = this->head; i != nullptr; i = i->next)
        result->addNode(i->coef, i->exp);
    for (auto i = source.head; i != nullptr; i = i->next)
        result->addNode(i->coef, i->exp);
    result->cleanNode();
    return *result;
}

Poly Poly::operator*(const Poly source) {
    Poly* result = new Poly();
    for (auto i = source.head; i != nullptr; i = i->next)
        for (auto j = this->head; j != nullptr; j = j->next)
            result->addNode(i->coef * j->coef, i->exp + j->exp);
    result->cleanNode();
    return *result;
}

ostream& operator<<(ostream& output, const Poly& source) {
    for (auto i = source.head; i != nullptr; i = i->next)
        output << i->coef << " " << i->exp << endl;
    if (source.head == nullptr)
        output << "0 0" << endl;
    return output;
}