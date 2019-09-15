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
    void addNode(int _coef, int _exp) {
        Term* newTerm = new Term(_coef, _coef, nullptr);
        if (this->head) {
            head->next = newTerm;
        } else {
            head = newTerm;
        }
    }

   private:
    Term* head = nullptr;
};

class Poly {
   public:
    Poly operator+(const Poly);
    Poly operator*(const Poly);

   private:
    int length;
};

int main() {
    int p, q;
    return 0;
}