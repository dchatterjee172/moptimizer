#include "parser.hpp"
using namespace std;
MFparser::MFparser(){}
		
MFparser::MFparser(string filenameT){
	const char *temp=filenameT.c_str();
	Filetoparse.open(temp);
	parsefilelinecount=0;
	filename=filenameT;
	synbad=true;
	sembad=true;
}
		
MFparser::~MFparser(){
	Filetoparse.close();
	syntaxerror.clear();
	for(int i=0;i<Lout.size();i++){
		Lout[i].clear();
	}
	Lout.clear();
}
		
void MFparser::MFopen(string filenameT){
	const char *temp=filenameT.c_str();
	if(Filetoparse.is_open()){
		Filetoparse.close();
	}
	Filetoparse.open(temp);
	parsefilelinecount=0;
	filename=filenameT;
	syntaxerror.clear();
	for(int i=0;i<Lout.size();i++){
		Lout[i].clear();
	}
	Lout.clear();
	synbad=true;
	sembad=true;
}
		
bool MFparser::MFfileopensuccess(){
	if(Filetoparse.is_open()){
		return true;
	}
	return false;
}
		
void MFparser::MFlexicalanalzye(){
	vector<Lexem> temp;
	string line,previousvalue,previouslexem;
	while(getline(Filetoparse,line)){
		if(Filetoparse.eof()) break;
		parsefilelinecount++;
		for(int i=0;i<line.size();i++){
			if(isspace(line[i]))
				continue;
			if(line[i]=='/' || line[i]=='*'||line[i]=='+'||line[i]=='-'||line[i]=='^'){
				if(previousvalue.size()==0&&line[i]=='-'&&!(previouslexem=="BR_C"||previouslexem=="VAR"||previouslexem=="NUM")){
					previousvalue.push_back(line[i]);
					temp.push_back(Lexem(previousvalue,"OP_U",parsefilelinecount));
					previousvalue.clear();
					previouslexem="OP_U";
					//previouslexem.clear();
					continue;
				}
				if(previouslexem=="VAR"&&previousvalue=="sin"||previousvalue=="cos"||previousvalue=="exp"||previousvalue=="fabs"||previousvalue=="log"||previousvalue=="log10"){
					temp.push_back(Lexem(previousvalue,"FUN",parsefilelinecount));
				}
				else if(previousvalue.size()!=0){
					temp.push_back(Lexem(previousvalue,previouslexem,parsefilelinecount));
				}
				previousvalue.clear();
				previousvalue.push_back(line[i]);
				temp.push_back(Lexem(previousvalue,"OP",parsefilelinecount));
				previousvalue.clear();
				previouslexem="OP";
				//previouslexem.clear();
			}
			else if(line[i]=='='){
				if(previouslexem=="VAR"&&previousvalue=="sin"||previousvalue=="cos"||previousvalue=="exp"||previousvalue=="fabs"||previousvalue=="log"||previousvalue=="log10"){
					temp.push_back(Lexem(previousvalue,"FUN",parsefilelinecount));
				}
				else if(previousvalue.size()!=0){
					temp.push_back(Lexem(previousvalue,previouslexem,parsefilelinecount));
				}
				temp.push_back(Lexem("=","OP_EQ",parsefilelinecount));
				previousvalue.clear();
				previouslexem="OP_EQ";
				//previouslexem.clear();
			}
			else if(line[i]=='.'){
				previousvalue.push_back(line[i]);
			}
			else if(line[i]=='('){
				if(previouslexem=="VAR"&&previousvalue=="sin"||previousvalue=="cos"||previousvalue=="exp"||previousvalue=="fabs"||previousvalue=="log"||previousvalue=="log10"){
					temp.push_back(Lexem(previousvalue,"FUN",parsefilelinecount));
				}
				else if(previousvalue.size()!=0){
					temp.push_back(Lexem(previousvalue,previouslexem,parsefilelinecount));
				}
				temp.push_back(Lexem("(","BR_O",parsefilelinecount));
				previousvalue.clear();
				previouslexem="BR_O";
				//previouslexem.clear();
			}
			else if(line[i]==')'){
				if(previouslexem=="VAR"&&previousvalue=="sin"||previousvalue=="cos"||previousvalue=="exp"||previousvalue=="fabs"||previousvalue=="log"||previousvalue=="log10"){
					temp.push_back(Lexem(previousvalue,"FUN",parsefilelinecount));
				}
				else if(previousvalue.size()!=0){
					temp.push_back(Lexem(previousvalue,previouslexem,parsefilelinecount));
				}
				temp.push_back(Lexem(")","BR_C",parsefilelinecount));
				previousvalue.clear();
				previouslexem="BR_C";
				//previouslexem.clear();
			}
			else if(isalpha(line[i])){
				previousvalue.push_back(line[i]);
				previouslexem="VAR";
			}
			else if(isdigit(line[i])){
				if(previousvalue.size()==0){
					previouslexem="NUM";
					previousvalue.push_back(line[i]);
				}
				else if(previousvalue.size()==1&&previousvalue[0]=='.'){
					previouslexem="NUM";
					previousvalue.push_back(line[i]);
				}
				else{
					previousvalue.push_back(line[i]);
				}
			}
			else{////////////////
				previousvalue.push_back(line[i]);
				//previouslexem="UNKN";
				temp.push_back(Lexem(previousvalue,"UNKN",parsefilelinecount));
				previousvalue.clear();
				//previouslexem.clear();
			}
			if(i==line.size()-1){
				if(previousvalue.size()!=0)
					temp.push_back(Lexem(previousvalue,previouslexem,parsefilelinecount));
				previousvalue.clear();
			}
		}
		if(previouslexem.size()!=0){
			Lout.push_back(temp);
			previouslexem.clear();
		}
		temp.clear();
	}
}
		
bool MFparser::MFsyntaxanalysis(){
	int bracketchk=0;
	string previouslexem,error;
	bool brackf=true;
	if(Lout.size()==0)	return false;
	syntaxerror.clear();
	for(int i=0;i<Lout.size();i++){
		bracketchk=0;
		brackf=true;
		for(int j=0;j<Lout[i].size();j++){
			if(j==0){
				if(Lout[i][j].lexem!="VAR"){
					previouslexem=Lout[i][j].lexem;
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Expected a funtion name in the first of the line in the place of "+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				else{
					previouslexem="VAR";
				}
				continue;
			}
			else if(j==1){
				if(Lout[i][j].lexem!="OP_EQ"){
					previouslexem=Lout[i][j].lexem;
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Expected a = in the second of the line in the place of "+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				else{
					previouslexem="OP_EQ";
				}
				continue;
			}
			if(Lout[i][j].lexem=="VAR"){
				if(!(previouslexem=="OP"||previouslexem=="OP_EQ"||previouslexem=="OP_U"||previouslexem=="BR_O"||previouslexem=="BR_C")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="NUM"){
				if(!(previouslexem=="OP"||previouslexem=="OP_EQ"||previouslexem=="OP_U"||previouslexem=="BR_O")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="OP_EQ"){
				error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
				syntaxerror.push_back(error);
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="OP"){
				if(!(previouslexem=="VAR"||previouslexem=="NUM"||previouslexem=="OP_U"||previouslexem=="BR_C")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="OP_U"){
				if(!(previouslexem=="OP"||previouslexem=="OP_EQ"||previouslexem=="BR_O")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="FUN"){
				if(!(previouslexem=="OP"||previouslexem=="OP_EQ"||previouslexem=="OP_U"||previouslexem=="BR_O")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
			}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="BR_O"){
				if(!(previouslexem=="OP"||previouslexem=="OP_EQ"||previouslexem=="OP_U"||previouslexem=="BR_O"||previouslexem=="FUN")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				else{
					bracketchk++;
				}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="BR_C"){
				if(!(previouslexem=="VAR"||previouslexem=="NUM"||previouslexem=="BR_C")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
				else{
					bracketchk--;
				}
				previouslexem=Lout[i][j].lexem;
			}
			else if(Lout[i][j].lexem=="UNKN"){
				error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value+"  cannot identify "+Lout[i][j].value;
				syntaxerror.push_back(error);
				previouslexem=Lout[i][j].lexem;
			}
			if(bracketchk<0){
				brackf=false;
			}
			if(j==Lout[i].size()-1){
				if(!(previouslexem=="VAR"||previouslexem=="NUM"||previouslexem=="BR_C")){
					error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j-2].value+Lout[i][j-1].value+Lout[i][j].value;
					syntaxerror.push_back(error);
				}
			}
		}
		if(bracketchk!=0||!brackf){
			error=filename+":"+to_string(Lout[i][0].linenumber)+" Error: Brackets are not placed right";
			syntaxerror.push_back(error);
			//cout<<"bR"<<bracketchk<<endl;
		}
	}
	if(syntaxerror.size()==0){
		synbad=false;
		return true;
	}
	else{
		//isbad=true;
		return false;
	}
}
		
void MFparser::MFerror(){
	for(int i=0;i<syntaxerror.size();i++){
		cout<<syntaxerror[i]<<endl;
	}	
}

vector<vector<Lexem> > MFparser::MFgetparseroutput(){
	if(synbad||sembad){
		Lout.clear();
		return Lout;
	}
	return Lout;
}
		
void MFparser::debugprintL(){
	for(int i=0;i<Lout.size();i++){
		cout<<Lout[i].size()<<" ";
		for(int j=0;j<Lout[i].size();j++){
			cout<<Lout[i][j].lexem<<"( "<<Lout[i][j].value<<" )"<<"  ";
		}
		cout<<endl;
	}
	cout<<Lout.size()<<endl;
}

bool MFparser::bad(){
	return (synbad||sembad);
}

bool  MFparser::MFsemanticanalysis(){
	if(synbad)
		return false;
	vector<Lexem> definedvar;
	string error;
	for(int i=0;i<Lout.size();i++){
		definedvar.push_back(Lout[i][0]);
		for(int j=2;j<Lout[i].size();j++){
			if(Lout[i][j].lexem=="VAR"&&Lout[i][j].value==definedvar[i].value){
				error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j].value+" is used in the equation which is equal to lhs variable";
				syntaxerror.push_back(error);
			}
		}
	}
	for(int i=0;i<Lout.size();i++){
		for(int j=2;j<Lout[i].size();j++){
			if(Lout[i][j].lexem=="VAR"){
				for(int k=definedvar.size()-1;k>i;k--){
					if(Lout[i][j].value==definedvar[k].value){
						error=filename+":"+to_string(Lout[i][j].linenumber)+" Error: "+Lout[i][j].value+" is used here but defined at line number "+to_string(definedvar[k].linenumber);
						syntaxerror.push_back(error);
					}
				}
			}
		}
	}
	if(syntaxerror.size()==0){
		sembad=false;
		return true;
	}
	else{
		return false;
	}
}

