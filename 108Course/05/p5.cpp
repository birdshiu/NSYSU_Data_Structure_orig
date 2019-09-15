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
    const bool hasNext() {
        return this->next != nullptr;
    }
};

class Chain {
   public:
    Chain() {
        this->head = nullptr;
    }
    pair<Term*, bool> findExp(int _exp);
    void addNode(int _coef, int _exp);
    void cleanNode();

   protected:
    Term* head = nullptr;
};

class Poly : public Chain {
   public:
    Poly operator+(const Poly);
    Poly operator*(const Poly);
};

int main() {
    int p, q;
    while (cin >> p >> q && p && q) {
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
        current = current->next
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
        } else if (insertPlace.first == this->head) {
            newTerm->next = this->head;
            this->head = newTerm;  // is the head of the Chain aka newTerm->exp is greater than the head
        } else if (insertPlace.first->next) {
            newTerm->next = insertPlace.first->next;
            insertPlace.first->next = newTerm;  // the middle of the Chain
        } else {
            insertPlace->next = newTerm;  // the tail
        }
    } else {
        this->head = newTerm;
    }
}

void Chain::cleanNode() {
    auto preverious = this->head;
    for (auto current = this->head; current; current = current->next) {
        if (current->coef) {    // if the coefficeint is not zero
            preverious = current;
        } else {
            preverious->next = current->next;
        }
    }
}