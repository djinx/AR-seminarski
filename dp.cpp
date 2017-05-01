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
  
  if (c.find(l) != c.end())
    return true;
  else
    return false;
}

bool DPSolve::DPSolve::resolution(Var v, const Clause & c1, const Clause & c2, const Clause & r) {
  
  return true;
  
}

bool DPSolve::eliminate(Var v) {
  
  FormulaCNF nf;
  
  Literal pv = litFromVar(v, POSITIVE);
  Literal nv = litFromVar(v, NEGATIVE);

  // For every clause c1 from F
    for(const Clause & c1 : _formula){	       
	if(!contains(c1, pv)){
	    if(!contains(c1, nv))
	      nf.insert(c1);

	    // If clause does not contain v we skip it
	    continue;
	  }

	// If clause c1 contains v
	for(const Clause & c2 : _formula){

	    // we find clause c2 that contains nv
	    if(!contains(c2, nv))
	      continue;

	    // apply resolution for v
	    Clause r;
	    // if not tautology
	    if(resolution(v, c1, c2, r)){
		if(!r.empty())
		  nf.insert(r);
		else return false;
	      }
	  }
      }
    _formula = move(nf);
    return true;
  
  
  return true;
  
}

bool DPSolve::checkIfSat() {
  
  return true;
  
}

int main () {
  
  
  
  cout << "Ne radi nistaa!! " << endl;
  return 0;
}