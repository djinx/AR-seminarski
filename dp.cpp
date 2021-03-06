#include "dp.hpp"

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
  
  if (c.find(l) != c.end()) {
    return true;
	}
  else {
    return false;
	}
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

	// za svaku klauzu c1 iz F
	for(const Clause & c1 : _formula){
	// ako klauza sadrzi pozitivan v i ne sadrzi negativan v dodajemo je u formulu
		if(!contains(c1, pv)){
			if(!contains(c1, nv)) {
				nf.insert(c1);
			}
			// ako klauza ne sadrzi v preskacemo je
			continue;
		}

		// ako klauza c1 sadrzi v
		for(const Clause & c2 : _formula){

			// pronalazimo klauzu c2 koja sadrzi nv
			if(!contains(c2, nv)) {
			  continue;
			}
			
			// primenjujemo rezoluciju na v
			Clause r;
			// ako nije tautologija
			if(resolution(v, c1, c2, r)){
				if(!r.empty()) {
					nf.insert(r);
				}
				else { 
					return false;
				}
			}
		}
	}
	
	_formula = move(nf);
	return true;
}

bool DPSolve::checkIfSat() {
  
  for(unsigned i = 0; i < _num; i++) {
		if(!eliminate(i)) {
			return false;
		}
	}
	
	return true;
}

int skipSpaces(istream & istr){
  int c;
  while((c = istr.get()) == ' ' || c == '\t' || c == '\n');
	return c;
}

void skipRestOfLine(istream & istr){
  while(istr.get() != '\n');
}

bool inDimacs(FormulaCNF & f, unsigned & num_of_vars, istream & istr)
{
  unsigned num_of_clauses;
  int c;

  // Preskace komentare
  while((c = skipSpaces(istr)) == 'c')
    skipRestOfLine(istr);

  // Cita liniju p cnf nvars nclauses
  if(c != 'p') {
    return false;
	}
  else {
		string s;
		istr >> s;
		if(s != "cnf") {
			return false;
		}
		istr >> num_of_vars;
		istr >> num_of_clauses;
	}

  // Citamo klauze
  for(unsigned i = 0; i < num_of_clauses; i++) {
		Clause c;
		int n;
		istr >> n; 
		while(!istr.eof() && !istr.fail() && n != 0) {
			c.insert(litFromInt(n));
			istr >> n;
		}
      
		if(istr.eof() || istr.fail()) {
			return false;
		}
		
		f.insert(c);
	}
  
	return true;
}

int main () {
  unsigned num_of_vars;

  FormulaCNF f;
  
  if(!inDimacs(f, num_of_vars, cin)){
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
  
  return 0;
}