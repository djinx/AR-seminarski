#include "fol.hpp"

//*********************************************
// Pomocne funkcije za baratanje listama 
 /* Funkcija nadovezuje dve liste */
template <typename T>
T concatLists(const T & c1, const T & c2) {
  T c = c1;
  
  for(auto it = c2.begin(), it_end = c2.end(); it != it_end; ++it){
    c.push_back(*it);
  }
  return c;
}


/* Funkcija nadovezuje svaku listu literala iz c1 sa svakom listom literala
   iz c2 i sve takve liste literala smesta u rezultujucu listu listi c */
LiteralListList makePairs(const LiteralListList & c1, 
			  const LiteralListList & c2) {
  LiteralListList c;
  for(auto & l1 : c1) {
    for(auto & l2 : c2) {
      c.push_back(concatLists(l1, l2));    
	}
  }
  return c;
}

//*********************************************



// funckija nnf() za klasu Not
Formula Not::nnf() {
  // ~~A === A
  if(_op->getType() == T_NOT){
	Not * not_op = (Not *) _op.get();
	return not_op->getOperand()->nnf();
  }
  // ~(A & B) === ~A | ~B
  else if(_op->getType() == T_AND) {
	And *and_op = (And *) _op.get();
	return make_shared<Or>(make_shared<Not>(and_op->getOperand1())->nnf(),
							make_shared<Not>(and_op->getOperand2())->nnf());
  }
  // ~(A | B) === ~A & ~B
  else if(_op->getType() == T_OR) {
	Or *or_op = (Or *) _op.get();
	return make_shared<And>(make_shared<Not>(or_op->getOperand1())->nnf(),
							make_shared<Not>(or_op->getOperand2())->nnf());
  }
  // ~(A => B) === A & ~B
  else if(_op->getType() == T_IMP) {
	Imp *imp_op = (Imp *) _op.get();
	return make_shared<Or>(imp_op->getOperand1()->nnf(),
							make_shared<Not>(imp_op->getOperand2())->nnf());
  }
  // ~(A <=> B) === (A & ~B) | (~A & B)
  else if(_op->getType() == T_IFF) {
	Iff *iff_op = (Iff*) _op.get();
	return make_shared<Or>(make_shared<And>(iff_op->getOperand1()->nnf(),
					  make_shared<Not>(iff_op->getOperand2())->nnf()),
				  make_shared<And>(make_shared<Not>(iff_op->getOperand1()->nnf()),
									iff_op->getOperand2()->nnf()));
  }
  // formula je vec u nnf-u
  else {
	return shared_from_this();
  }
}
/*
LiteralListList And::listCNF() {
	// nadovezujemo podformule i dobijamo formulu u CNF
	LiteralListList c1 = _op1->listCNF();
	LiteralListList c2 = _op2->listCNF();
	
	return concatLists(c1, c2);
}

LiteralListList Or::listCNF() {
	// disjnkcija se pretvara u cnf tako sto se podformule pomnoze
	LiteralListList c1 = _op1->listCNF();
	LiteralListList c2 = _op2->listCNF();
	
	return makePairs(c1, c2);
	
  }
  
  */