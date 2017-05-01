#include "dp.hpp"
#include "fol.hpp"


extern int yyparse();

Literal litFromVar(Var v, Polarity p) {
  return p == POSITIVE ? v << 1 : (v << 1) | 1;
}

Var varFromLit(Literal l){
  return l >> 1;
}

bool isPositive(Literal l){
  return !(l & 1);
}

bool isNegative(Literal l){
  return l & 1;
}

Literal oppositeLiteral(Literal l) {
  return l ^ 1;
}


DPSolve::DPSolve(const FormulaCNF & f, unsigned num)
  :_formula(f), _num(num)
  {}

bool DPSolve::contains(const Clause & c, Literal l) {
  
  return true;
  
}

bool DPSolve::DPSolve::resolution(Literal l, Clause & c1, Clause & c2, Clause & r) {
  
  return true;
  
}

bool DPSolve::eliminate(Var v) {
  
  return true;
  
}

bool DPSolve::checkIfSat() {
  
  return true;
  
}

int main () {
  
  
  
  cout << "Ne radi nistaa!! " << endl;
  return 0;
}