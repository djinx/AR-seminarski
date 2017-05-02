#ifndef DP

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <fstream>

using namespace std;

typedef unsigned Var;
typedef unsigned Literal;

enum Polarity {POSITIVE, NEGATIVE};


// Konverzija promenljive u literal datog polariteta
Literal litFromVar(Var v, Polarity p);

/* Dobijanje varijable iz literala proizvoljnog polariteta */
Var varFromLit(Literal l);

/* Ispitivanje da li je literal pozitivan (ako je broj paran, tj. ako
   je najnizi bit 0) */
bool isPositive(Literal l);

/* Ispitivanje da li je literal negativan (ako je broj neparan, tj. ako
   je najnizi bit 1) */
bool isNegative(Literal l);

/* Dobijanje suprotnog literala (invertovanjem najnizeg bita) */
Literal oppositeLiteral(Literal l);

/* Klauza ce biti predstavljena vektorom (dinamickim nizom) literala */
typedef set<Literal> Clause;

/* Formula je vektor (niz) klauza */
typedef set<Clause> FormulaCNF;

int intFromLit(Literal l);

Literal litFromInt(int i);


class DPSolve {
  
public:
  // konstruktor
  DPSolve(const FormulaCNF & f, unsigned num);
  
  // funkcija koja proverava da li klauza c sadrzi literal l
  bool contains(const Clause & c, Literal l);
  
  /* funkcija koja primenjuje pravilo rezolucije nad klauzama c1 i c2 
	 po literalu l - jedna sadrzi l a druga ~l
	 prosledjuje se v da bismo lakse dobili l i ~l
	 r je rezultujuca klauza koja ne sadrzi l
  */
  bool resolution(Var v, const Clause & c1, const Clause & c2, Clause & r);
  
  // funkcija koja uklanja iskazno slovo v iz formule primenom pravila rezolucije
  bool eliminate(Var v);
  
  // funkcija koja proverava zadovoljivost formule
  bool checkIfSat();
  
  // Funkcija koja preskace beline pri ucitavanju iz dimacs formata
  int skipSpaces(ifstream & istr);
  
  // Funkcija koja preskace ostatak reda
  int skipRestOfLine(ifstream & istr);
  
  // Funkcija koja ucitava DIMACS format
  bool inDimacs(FormulaCNF & f, unsigned & num_of_vars, ifstream & istr);
  
  
private:
  FormulaCNF _formula; // formula ciju zadovoljivost proveravamo
  unsigned _num; // broj promenljivih u formuli
  
};



#endif