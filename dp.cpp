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

bool DPSolve::DPSolve::resolution(Var v, Clause & c1, Clause & c2, Clause & r) {
  
  // pravimo novu klauzu r i iz nje izbacujemo literal l
  r = c1;
  r.erase(litFromVar(v, POSITIVE));
  
  Literal k = litFromVar(v, NEGATIVE);
  
  // zatim u r ubacujemo sve literale iz c2 koji nisu ~l
  for(Literal l : c2){
	if( l != k ){
	  // proveravamo da li u klauzi r postoji literal suprotan literalu l
	  if(r.find(oppositeLiteral(l)) == r.end() ){
		r.insert(l);
	  }
	  else{
		// ako r sadrzi suprotan literal onda je r tautologija
		return false;
	  }
	}
  }
  
  // ima jos literala u klauzi tj. nije tautologija
  return true;
}

bool DPSolve::eliminate(Var v) {
  
  return true;
  
}

bool DPSolve::checkIfSat() {
  
  return true;
  
}

int main () {
  unsigned num_of_vars;

  Variable p = 0;
  Variable q = 1;
  Variable r = 2;
  Literal pp = litFromVar(p, POSITIVE);
  Literal np = litFromVar(p, NEGATIVE);
  Literal pq = litFromVar(q, POSITIVE);
  Literal nq = litFromVar(q, NEGATIVE);
  Literal pr = litFromVar(r, POSITIVE);
  Literal nr = litFromVar(r, NEGATIVE);
  
  Formula f = { { pp, nq, pr }, { np, nq }, { pq, pp, nr }, { pq, np, nr } };

  // Ako zelimo da citamo iz dimacs fajla, tada treba da otkomentarisemo ovo
  /*  if(!readDIMACS(f, num_of_vars))
    {
      cerr << "Error reading input file" << endl;
      exit(1);
    }
  */
  
  DPSolve solver(f, num_of_vars);

  if(!solver.checkSat()) {
	cout << "UNSAT" << endl;
  }
  else {
	cout << "SAT" << endl;
  }
  
  return 0;
}