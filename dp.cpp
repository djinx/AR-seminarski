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


int intFromLit(Literal l){
  return isPositive(l) ? (int)varFromLit(l) + 1 : -(int)(varFromLit(l) + 1);
}

Literal litFromInt(int i){
  return i > 0 ? litFromVar(i - 1, POSITIVE) : litFromVar(-i - 1, NEGATIVE);
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


bool DPSolve::DPSolve::resolution(Var v, const Clause & c1, const Clause & c2, Clause & r) {

  
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
  
  for(unsigned i = 0; i < _num; i++)
      if(!eliminate(i))
	return false;
    return true;
  
}

int skipSpaces(){
  int ch;
  while((ch = cin.get()) == ' ' || ch == '\t' || ch == '\n');
  return ch;
}

void skipRestOfLine(){
  while(cin.get() != '\n');
}

bool inDimacs(FormulaCNF & f, unsigned & num_of_vars){
  unsigned num_of_clauses;
  int c;

  // Skip comment lines
  while((c = skipSpaces()) == 'c'){
    skipRestOfLine();
  }

  // read p line
  if(c != 'p'){
    return false;
  }
  else{
      string s;
      cin >> s;
      if(s != "cnf")
	return false;
      
      cin >> num_of_vars;
      cin >> num_of_clauses;
    }

  // Read clauses
  for(unsigned i = 0; i < num_of_clauses; i++){
      Clause c;
      int n;
      cin >> n; 
      while(!cin.eof() && !cin.fail() && n != 0){
	  c.insert(litFromInt(n));
	  cin >> n;
	}
      
      if(cin.eof() || cin.fail()){
	return false;
      }

      f.insert(c);
    }
  return true;
}

int main () {
  unsigned num_of_vars;

  Var p = 0;
  Var q = 1;
  Var r = 2;
  Literal pp = litFromVar(p, POSITIVE);
  Literal np = litFromVar(p, NEGATIVE);
  Literal pq = litFromVar(q, POSITIVE);
  Literal nq = litFromVar(q, NEGATIVE);
  Literal pr = litFromVar(r, POSITIVE);
  Literal nr = litFromVar(r, NEGATIVE);
  
  FormulaCNF f = { { pp, nq, pr }, { np, nq }, { pq, pp, nr }, { pq, np, nr } };

  // Ako zelimo da citamo iz dimacs fajla, tada treba da otkomentarisemo ovo
    if(!inDimacs(f, num_of_vars)){
      cerr << "Error reading input file" << endl;
      exit(1);
    }
  
  DPSolve solver(f, num_of_vars);

  if(!solver.checkIfSat()) {
	cout << "UNSAT" << endl;
  }
  else {
	cout << "SAT" << endl;
  }
  
  FormulaCNF f1 = { { np, nq, pr }, {np, pq}, { pp }, {nr} };
  
  DPSolve solver1(f1, num_of_vars);

  if(!solver1.checkIfSat()) {
	cout << "UNSAT" << endl;
  }
  else {
	cout << "SAT" << endl;
  }
  
  return 0;
}