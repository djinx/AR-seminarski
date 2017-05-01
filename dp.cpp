#include "dp.hpp"


Literal litFromVar(Variable v, Polarity p) {
  return p == POSITIVE ? v << : (v << 1) | 1;
}

Variable varFromLit(Literal l){
  return l >> 1;
}

bool isPositive(Literal l);

bool isNegative(Literal l);

Literal oppositeLiteral(Literal l) {
  return l ^ 1;
}

typedef vector<Literal> Clause;
typedef vector<Clause> Formula;


DPSolve::DPSolve(const Formula & f, unsigned num);

bool DPSolve::contains(const Clause & c, Literal l);

bool DPSolve::DPSolve::resolution(Literal l, Clause & c1, Clause & c2, Clause & r);

bool DPSolve::eliminate(Variable v);

bool DPSolve::checkIfSat();


int main () {
  
  
  
  cout << "Ne radi nistaa!! " << endl;
  return 0;
}