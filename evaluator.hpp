#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP
#include "iostream"
#include "cstdlib"
#include "fstream"
#include "vector"
#include "stack"
#include "cmath"

#ifndef LEX
#define LEX
struct Lex{
	std::string lexem,value;
	int linenumber;
	Lex(std::string tempv,std::string templ,int templn){
		lexem=templ;
		value=tempv;
		linenumber=templn;
	}
};
typedef struct Lex Lexem;
#endif

struct Var{
	std::string variable;
	long double value;
	bool defined;
	Var(std::string tempvar,bool tempd){
		variable=tempvar;
		defined=tempd;
	}
};
 
typedef struct Var Variables;

struct RPN{
	std::vector<Lexem> rpn;
	std::string varname;
};

typedef struct RPN RPNline;

class MFfunction{
	std::vector<Variables> definedvar,undefinedvar;
	std::vector<std::vector<Lexem> > Lout;
	std::vector<RPNline> functionrpn;
	long double result;
	void createRPN();
	public:
		MFfunction();
		~MFfunction();
		void MFgetparseroutput(std::vector<std::vector<Lexem> >);
		std::vector<std::string> MFundefinedvar();
		long double MFevaluate(std::vector<long double>);
};		
#endif
