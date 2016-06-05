#ifndef _PARSER_HPP
#define _PARSER_HPP
#include "iostream"
#include "cctype"
#include "fstream"
#include "vector"
#include "string"
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
class MFparser{
	std::ifstream Filetoparse;
	std::vector<std::string> syntaxerror;
	std::vector<std::vector<Lexem> > Lout;
	int parsefilelinecount;
	std::string filename;
	bool synbad,sembad;
	public:
		MFparser();
		MFparser(std::string);
		~MFparser();
		void MFopen(std::string);
		bool MFfileopensuccess();
		void MFlexicalanalzye();
		bool MFsyntaxanalysis();
		void MFerror();
		std::vector<std::vector<Lexem> > MFgetparseroutput();
		void debugprintL();
		bool bad();
		bool MFsemanticanalysis();
};
#endif
