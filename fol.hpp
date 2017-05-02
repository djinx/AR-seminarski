#ifndef _FOL_H
#define _FOL_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

using namespace std;

typedef string FunctionSymbol;
typedef string PredicateSymbol;
typedef string Variable;


class BaseTerm;
typedef shared_ptr<BaseTerm> Term;



class BaseTerm : public enable_shared_from_this<BaseTerm> {

public:
  enum Type { TT_VARIABLE, TT_FUNCTION };
  virtual Type getType() const = 0;
  virtual void printTerm(ostream & ostr) const = 0;
  virtual ~BaseTerm() {}
};

class VariableTerm : public BaseTerm {
private:
  Variable _v;
public:
  VariableTerm(const Variable & v)
    :_v(v)
  {}

  virtual Type getType() const
  {
    return TT_VARIABLE;
  }

  const Variable & getVariable() const
  {
    return _v;
  }
  virtual void printTerm(ostream & ostr) const
  {
    ostr << _v;
  }
};

class FunctionTerm : public BaseTerm {
private:
  FunctionSymbol _f;
  vector<Term> _ops;

public:
  FunctionTerm(const FunctionSymbol & f, 
	       const vector<Term> & ops = vector<Term> ())
    :_f(f),
     _ops(ops)
  {}

  virtual Type getType() const
  {
    return TT_FUNCTION;
  }

  const FunctionSymbol & getSymbol() const
  {
    return _f;
  }

  const vector<Term> & getOperands() const
  {
    return _ops;
  }

  virtual void printTerm(ostream & ostr) const
  {
    ostr << _f;

    for(unsigned i = 0; i < _ops.size(); i++)
      {
	if(i == 0)
	  ostr << "(";
	_ops[i]->printTerm(ostr);
	if(i != _ops.size() - 1)
	  ostr << ",";
	else
	  ostr << ")";
      }
  }
};

class BaseFormula;
typedef shared_ptr<BaseFormula> Formula;

typedef vector<Formula> LiteralList;
typedef vector<LiteralList> LiteralListList;


template <typename T>
T concatLists(const T & c1, const T & c2);
LiteralListList makePairs(const LiteralListList & c1, 
			  const LiteralListList & c2);

class BaseFormula : public enable_shared_from_this<BaseFormula> {
  
public:

  enum Type { T_TRUE, T_FALSE, T_ATOM, T_NOT, 
	      T_AND, T_OR, T_IMP, T_IFF, T_FORALL, T_EXISTS };

  virtual void printFormula(ostream & ostr) const = 0;
  virtual Type getType() const = 0;
  virtual Formula simplify() = 0;
  virtual Formula nnf() = 0;
  virtual LiteralListList listCNF() = 0;
  virtual ~BaseFormula() {}
};



class AtomicFormula : public BaseFormula {
public:
  virtual Formula simplify() {
	// nemamo sta da pojednostavimo
	return shared_from_this();
  }
  
  virtual Formula nnf() {
	// atomicka formula je vec u nnf-u
	return shared_from_this();
  }
};


class LogicConstant : public AtomicFormula {

public:
};


class True : public LogicConstant {

public:
  virtual void printFormula(ostream & ostr) const
  {
    ostr << "true";
  }

  virtual Type getType() const
  {
    return T_TRUE;
  }

  virtual LiteralListList listCNF() {
	// prazan skup klauza se interpretira kao True
	return { };
  }
};


class False : public LogicConstant {

public:
  virtual void printFormula(ostream & ostr) const
  {
    ostr << "false";
  }

  virtual Type getType() const
  {
    return T_FALSE;
  }
  
  virtual LiteralListList listCNF() {
	// prazna klauza se interpretira kao False
	return {{}};
  }
};


class Atom : public AtomicFormula {
protected:
  PredicateSymbol _p;
  vector<Term> _ops;

public:
  Atom(const PredicateSymbol & p, 
       const vector<Term> & ops = vector<Term>())
    :_p(p),
     _ops(ops)
  {}

  const PredicateSymbol & getSymbol() const
  {
    return _p;
  }

  const vector<Term> & getOperands() const
  {
    return _ops;
  }

  virtual void printFormula(ostream & ostr) const
  {
    ostr << _p;
    for(unsigned i = 0; i < _ops.size(); i++)
      {
	if(i == 0)
	  ostr << "(";
	_ops[i]->printTerm(ostr);
	if(i != _ops.size() - 1)
	  ostr << ",";
	else
	  ostr << ")";
      }
  }

  virtual Type getType() const
  {
    return T_ATOM;
  }

  virtual LiteralListList listCNF() {
	// pozitivan atom se predstavlja listom klauza koja sadrzi jednu klauzu sa tim atomom
	return {{shared_from_this()}};
  }
  
};

class Equality : public Atom {
public:
  Equality(const Term & lop, const Term & rop)
    :Atom("=", vector<Term> ())
  {
    _ops.push_back(lop);
    _ops.push_back(rop);
  }
  
  const Term & getLeftOperand() const
  {
    return _ops[0];
  }
  
  const Term & getRightOperand() const
  {
    return _ops[1];
  }

  virtual void printFormula(ostream & ostr) const
  {
    _ops[0]->printTerm(ostr);
    ostr << " = ";
    _ops[1]->printTerm(ostr);
  }
};

class Disequality : public Atom {
public:
  Disequality(const Term & lop, const Term & rop)
    :Atom("~=", vector<Term> ())
  {
    _ops.push_back(lop);
    _ops.push_back(rop);
  }
  
  const Term & getLeftOperand() const
  {
    return _ops[0];
  }
  
  const Term & getRightOperand() const
  {
    return _ops[1];
  }

  virtual void printFormula(ostream & ostr) const
  {

    _ops[0]->printTerm(ostr);
    ostr << " ~= ";
    _ops[1]->printTerm(ostr);
  }
};


class UnaryConjective : public BaseFormula {
protected:
   Formula _op;
public:
  UnaryConjective(const Formula & op)
    :_op(op)
  {}

  const Formula & getOperand() const
  {
    return _op;
  }
};

class Not : public UnaryConjective {
public:
  Not(const Formula & op)
    :UnaryConjective(op)
  {}

  virtual void printFormula(ostream & ostr) const
  { 
    ostr << "~";
    Type op_type = _op->getType();

    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);

    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_NOT;
  }
  
  virtual Formula simplify() {
	Formula simplified = _op->simplify();
	
	// ~True = False
	if(simplified->getType() == T_TRUE) {
	  return make_shared<False>();
	}
	// ~False = True
	else if(simplified->getType() == T_FALSE) {
	  return make_shared<True>();
	}
	//nemamo sta da pojednostavimo
	else {
	  return make_shared<Not>(simplified);
	}
  }
  
  virtual Formula nnf();
  
  virtual LiteralListList listCNF() {
	// pozitivan atom se predstavlja listom klauza koja sadrzi jednu klauzu sa tim atomom
	return {{shared_from_this()}};
  }
};


class BinaryConjective : public BaseFormula {
protected:
   Formula _op1, _op2;
public:
  BinaryConjective(const Formula & op1, const Formula & op2)
    :_op1(op1),
     _op2(op2)
  {}

  const Formula & getOperand1() const
  {
    return _op1;
  }

  const Formula & getOperand2() const
  {
    return _op2;
  }
};


class And : public BinaryConjective {
public:
  And(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {
    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_OR || op1_type == T_IMP || 
       op1_type == T_IFF)
      ostr << "(";
    
    _op1->printFormula(ostr);
    
    if(op1_type == T_OR || op1_type == T_IMP || 
       op1_type == T_IFF)
      ostr << ")";

    ostr << " & ";

    if(op2_type == T_OR || op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_AND)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_OR || op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_AND)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_AND;
  }
  
  virtual Formula simplify() {
	Formula simplified1 = _op1->simplify();
	Formula simplified2 = _op2->simplify();
	
	//True & A === A
	if(simplified1->getType() == T_TRUE){
	  return simplified2;
	}
	//A & True === A
	else if(simplified2->getType() == T_TRUE){
	  return simplified1;
	}
	//A & False === False & A === False
	else if(simplified1->getType() == T_FALSE || 
			simplified2->getType() == T_FALSE
	){
	  return make_shared<False>();
	}
	//nemamo sta da pojednostavimo
	else{
	  make_shared<And>(simplified1, simplified2);
	}
  }
  
  virtual Formula nnf(){
  return make_shared<And>(_op1->nnf(), _op2->nnf());
  }
  
  virtual LiteralListList listCNF() {
	// nadovezujemo podformule i dobijamo formulu u CNF
	LiteralListList c1 = _op1->listCNF();
	LiteralListList c2 = _op2->listCNF();
	
	return concatLists(c1, c2);
  }
  
};



class Or : public BinaryConjective {
public:
  Or(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_IMP || op1_type == T_IFF)
      ostr << "(";
    
    _op1->printFormula(ostr);
    
    if(op1_type == T_IMP || op1_type == T_IFF)
      ostr << ")";

    ostr << " | ";

    if(op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_OR)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_IMP || 
       op2_type == T_IFF || op2_type == T_OR)
      ostr << ")";
  }

  virtual Type getType() const
  {
    return T_OR;
  }
  
  virtual Formula simplify(){
	
	Formula simplified1 = _op1->simplify();
	Formula simplified2 = _op2->simplify();
	
	//False | A === A
	if(simplified1->getType() == T_FALSE){
	  return simplified2;
	}
	//A | False === A
	else if(simplified2->getType() == T_FALSE){
	  return simplified1;
	}
	//A | True === True | A === True
	else if(simplified1->getType() == T_TRUE || 
			simplified2->getType() == T_TRUE
	){
	  return make_shared<True>();
	}
	//nemamo sta da pojednostavimo
	else{
	  make_shared<Or>(simplified1, simplified2);
	}
  }
  
  virtual Formula nnf() {
	return make_shared<Or>(_op1->nnf(), _op2->nnf());
  }
  
  virtual LiteralListList listCNF() {
	
	// disjnkcija se pretvara u cnf tako sto se podformule pomnoze
	LiteralListList c1 = _op1->listCNF();
	LiteralListList c2 = _op2->listCNF();
	
	return makePairs(c1, c2);
  }
  
};


class Imp : public BinaryConjective {
public:
  Imp(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}
  
  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();

    if(op1_type == T_IFF)
      ostr << "(";
    
    _op1->printFormula(ostr);
    
    if(op1_type == T_IFF)
      ostr << ")";

    ostr << " => ";

    if(op2_type == T_IMP || op2_type == T_IFF)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_IMP || op2_type == T_IFF)
      ostr << ")";
  }
  
  virtual Type getType() const
  {
    return T_IMP;
  }
  
 
  virtual Formula simplify(){
	
	Formula simplified1 = _op1->simplify();
	Formula simplified2 = _op2->simplify();
	
	// True => A === A
	if(simplified1->getType() == T_TRUE){
	  return simplified2;
	}
	// A => True === True
	else if(simplified2->getType() == T_TRUE){
	  return make_shared<True>();
	}
	// False => A === True
	else if(simplified1->getType() == T_FALSE){
	  return make_shared<True>();
	}
	// A => False === ~A
	else if(simplified2->getType() == T_TRUE) {
	  return make_shared<Not>(simplified1);
	}
	// nemamo sta da pojednostavimo
	else {
	  return make_shared<Imp>(simplified1, simplified2);
	}
  }
  
  virtual Formula nnf() {
	// ~(A => B) === ~A | B
	return make_shared<Or>(make_shared<Not>(_op1)->nnf(), _op2->nnf());
  }

  virtual LiteralListList listCNF() {
	// u ovom trenutku ne bi trebalo da postoji implikacija jer je primenjen nnf
	throw "CNF not aplicable";
  }
  
};



class Iff : public BinaryConjective {

public:
  Iff(const Formula & op1, const Formula & op2)
    :BinaryConjective(op1, op2)
  {}

  virtual void printFormula(ostream & ostr) const
  {

    Type op1_type = _op1->getType();
    Type op2_type = _op2->getType();
    
    _op1->printFormula(ostr);
    
    ostr << " => ";

    if(op2_type == T_IFF)
      ostr << "(";
    
    _op2->printFormula(ostr);

    if(op2_type == T_IFF)
      ostr << ")";

  }
  
  virtual Type getType() const
  {
    return T_IFF;
  }
  
  virtual Formula simplify() {
	Formula simplified1 = _op1->simplify();
	Formula simplified2 = _op2->simplify();
	
	// False <=> False === True
	if(simplified1->getType() == T_FALSE && 
	   simplified2->getType() == T_FALSE) {
	  
	  return make_shared<True>();
	}
	// True <=> A === A
	else if(simplified1->getType() == T_TRUE) {
	  return simplified2;
	}
	// A <=> True  === A
	else if(simplified2->getType() == T_TRUE) {
	  return simplified1;
	}
	// False <=> A === ~A
	else if(simplified1->getType() == T_FALSE) {
	  return make_shared<Not>(simplified2);
	}
	// A <=> False === ~A
	else if(simplified2->getType() == T_FALSE) {
	  return make_shared<Not>(simplified1);
	}
	// nemamo sta da pojednostavimo
	else{
	  return make_shared<Iff>(simplified1, simplified2);
	}
  }
  
  virtual Formula nnf() {
	// ~(A <=> B) === (~A | B) & (~B | A)
	return make_shared<And>(make_shared<Or>(make_shared<Not>(_op1)->nnf(), _op2->nnf()),
			make_shared<Or>(make_shared<Not>(_op2)->nnf(), _op1->nnf()));
  }
  
  virtual LiteralListList listCNF() {
	// u ovom trenutku ne bi trebalo da postoji implikacija jer je primenjen nnf
	throw "CNF not aplicable";
  }
  
};

class Quantifier : public BaseFormula {
protected:
  Variable _v;
  Formula  _op;
public:
  Quantifier(const Variable & v, const Formula & op)
    :_v(v),
     _op(op)
  {}

  const Variable & getVariable() const
  {
    return _v;
  }

  const Formula & getOperand() const
  {
    return _op;
  }

};

class Forall : public Quantifier {
public:
  Forall(const Variable & v, const Formula & op)
    :Quantifier(v, op)
  {}

  virtual Type getType() const
  {
    return T_FORALL;
  }
  virtual void printFormula(ostream & ostr) const
  {
    cout << "![" << _v << "] : ";

    Type op_type = _op->getType();
    
    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);

    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }
  
  virtual Formula simplify() {
	return shared_from_this();
  }
  
  virtual Formula nnf() {
	return shared_from_this();
  }
  
  virtual LiteralListList listCNF() {
	return {{shared_from_this()}};
  }
  
};

class Exists : public Quantifier {
public:
  Exists(const Variable & v, const Formula & op)
    :Quantifier(v, op)
  {}

  virtual Type getType() const
  {
    return T_EXISTS;
  }

  virtual void printFormula(ostream & ostr) const
  {
    cout  << "?[" << _v << "] : ";
   
    Type op_type = _op->getType();
    
    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << "(";

    _op->printFormula(ostr);
    
    if(op_type == T_AND || op_type == T_OR || 
       op_type == T_IMP || op_type == T_IFF)
      ostr << ")";
  }
  
  
  virtual Formula simplify() {
	return shared_from_this();
  }
  
  virtual Formula nnf() {
	return shared_from_this();
  }
  
  virtual LiteralListList listCNF() {
	return {{shared_from_this()}};
  }
};

inline
ostream & operator << (ostream & ostr, const Term & t)
{
  t->printTerm(ostr);
}

inline
ostream & operator << (ostream & ostr, const Formula & f)
{
  f->printFormula(ostr);
}

extern Formula parsed_formula;

#endif // _FOL_H
