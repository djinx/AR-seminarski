#ifndef DP

#include <iostream>
#include <vector>
#include <cstdlib>


using namespace std;

typedef unsigned Variable;
typedef unsigned Literal;

enum Polarity {POSITIVE, NEGATIVE};


/* Dobijanje literala datog polariteta iz varijable */
Literal litFromVar(Variable v, Polarity p);

/* Dobijanje varijable iz literala proizvoljnog polariteta */
Variable varFromLit(Literal l);

/* Ispitivanje da li je literal pozitivan (ako je broj paran, tj. ako
   je najnizi bit 0) */
bool isPositive(Literal l);

/* Ispitivanje da li je literal negativan (ako je broj neparan, tj. ako
   je najnizi bit 1) */
bool isNegative(Literal l);

/* Dobijanje suprotnog literala (invertovanjem najnizeg bita) */
Literal oppositeLiteral(Literal l);

/* Klauza ce biti predstavljena vektorom (dinamickim nizom) literala */
typedef vector<Literal> Clause;

/* Formula je vektor (niz) klauza */
typedef vector<Clause> Formula;


class DPSolve {
  
public:
  // konstruktor
  DPSolve(const Formula & f, unsigned num);
  
  // funkcija koja proverava da li klauza c sadrzi literal l
  bool contains(const Clause & c, Literal l);
  
  /* funkcija koja primenjuje pravilo rezolucije nad klauzama c1 i c2 
	 po literalu l - jedna sadrzi l a druga ~l
	 r je rezultujuca klauza koja ne sadrzi l
  */
  bool resolution(Literal l, Clause & c1, Clause & c2, Clause & r);
  
  // funkcija koja uklanja iskazno slovo v iz formule primenom pravila rezolucije
  bool eliminate(Variable v);
  
  // funkcija koja proverava zadovoljivost formule
  bool checkIfSat();
  
  
  
private:
  Formula _formula; // formula ciju zadovoljivost proveravamo
  unsigned _num; // broj promenljivih u formuli
  
};



#endif