#include "evaluator.hpp"
using namespace std;

int opprecedence(string op){
	if(op=="^")	return 4;
	else if(op=="/")	return 3;
	else if(op=="*")	return 3;
	else if(op=="+")	return 2;
	else if(op=="-")	return 2;
}

bool opassociavityleft(string op){
	if(op=="^") 	return false;
	else return true;
}

void MFfunction::createRPN(){
	RPNline temp;
	stack<Lexem> OPstack;
	functionrpn.clear();
	for(int i=0;i<Lout.size();i++){
		temp.rpn.clear();
		temp.varname=Lout[i][0].value;
		for(int j=2;j<Lout[i].size();j++){
			if(Lout[i][j].lexem=="NUM"||Lout[i][j].lexem=="VAR"){
				temp.rpn.push_back(Lout[i][j]);
				if(!OPstack.empty()&&OPstack.top().lexem=="OP_U"){
					temp.rpn.push_back(OPstack.top());
					OPstack.pop();
				}
			}
			else if(Lout[i][j].lexem=="FUN"){
				OPstack.push(Lout[i][j]);
			}
			else if(Lout[i][j].lexem=="OP"){
				if(OPstack.empty()){
					OPstack.push(Lout[i][j]);
				}
				else{
					while(!OPstack.empty()&&OPstack.top().lexem=="OP"){
						if((opassociavityleft(Lout[i][j].value)&&opprecedence(Lout[i][j].value)<=opprecedence(OPstack.top().value))||(!opassociavityleft(Lout[i][j].value)&&opprecedence(Lout[i][j].value)<opprecedence(OPstack.top().value))){
							temp.rpn.push_back(OPstack.top());
							OPstack.pop();
						}
						else break;
					}
					OPstack.push(Lout[i][j]);
				}
			}
			else if(Lout[i][j].lexem=="OP_U"){
				OPstack.push(Lout[i][j]);
			}
			else if(Lout[i][j].lexem=="BR_O"){
				OPstack.push(Lout[i][j]);
			}
			else if(Lout[i][j].lexem=="BR_C"){
				while(OPstack.top().lexem!="BR_O"){
					temp.rpn.push_back(OPstack.top());
					OPstack.pop();
				}
				OPstack.pop();
				if(!OPstack.empty()&&(OPstack.top().lexem=="FUN"||OPstack.top().lexem=="OP_U")){
					temp.rpn.push_back(OPstack.top());
					OPstack.pop();
				}
			}
		}
		while(!OPstack.empty()){
			temp.rpn.push_back(OPstack.top());
			OPstack.pop();
		}
		functionrpn.push_back(temp);
		/*for(int l=0;l<temp.rpn.size();l++){
			cout<<temp.rpn[l].value;
		}
		cout<<endl;*/
	}
}

MFfunction::MFfunction(){}
MFfunction::~MFfunction(){
	definedvar.clear();
	undefinedvar.clear();
	functionrpn.clear();
	for(int i=0;i<Lout.size();i++){
		Lout[i].clear();
	}
	Lout.clear();
}
		
void MFfunction::MFgetparseroutput(vector<vector<Lexem> > templ){
	Lout=templ;
	definedvar.clear();
	undefinedvar.clear();
	for(int i=0;i<Lout.size();i++){
		definedvar.push_back(Variables(Lout[i][0].value,false));
	}
	this->MFundefinedvar();
	this->createRPN();
}
		
vector<string> MFfunction::MFundefinedvar(){
	vector<string> returnvar;
	if(undefinedvar.size()!=0){
		for(int i=0;i<undefinedvar.size();i++){
			returnvar.push_back(undefinedvar[i].variable);
		}
		return returnvar;
	}
	bool flag;
	int k;
	for(int i=0;i<Lout.size();i++){
		for(int j=2;j<Lout[i].size();j++){
			if(Lout[i][j].lexem=="VAR"){
				flag=true;
				for(k=0;k<undefinedvar.size();k++){
					if(Lout[i][j].value==undefinedvar[k].variable){
						flag=false;
						break;
					}
				}
				for(k=0;k<definedvar.size()&&flag;k++){
					if(Lout[i][j].value==definedvar[k].variable) break;
				}
				if(flag&&k==definedvar.size()){
					undefinedvar.push_back(Variables(Lout[i][j].value,false));
					returnvar.push_back(Lout[i][j].value);
				}
			}
		}
	}
	return returnvar;
}
long double MFfunction::MFevaluate(vector<long double> undefinedvarvalue){
	stack<long double> Eval;
	bool flag;
	long double temp1,temp2;
	if(functionrpn.size()==0&&undefinedvarvalue.size()!=undefinedvar.size()){
		return NAN;
	}	
	for(int i=0;i<undefinedvar.size();i++){
		undefinedvar[i].value=undefinedvarvalue[i];
		undefinedvar[i].defined=true;
	}
	for(int i=0;i<functionrpn.size();i++){
		for(int j=0;j<functionrpn[i].rpn.size();j++){
			if(functionrpn[i].rpn[j].lexem=="NUM"){
				Eval.push(stold(functionrpn[i].rpn[j].value));
			}
			else if(functionrpn[i].rpn[j].lexem=="OP_U"){
				temp1=Eval.top();
				Eval.pop();
				Eval.push(-temp1);
			}
			else if(functionrpn[i].rpn[j].lexem=="OP"){
				temp1=Eval.top();
				Eval.pop();
				temp2=Eval.top();
				Eval.pop();
				if(functionrpn[i].rpn[j].value=="+"){
					Eval.push(temp2+temp1);
				}
				else if(functionrpn[i].rpn[j].value=="-"){
					Eval.push(temp2-temp1);
				}
				else if(functionrpn[i].rpn[j].value=="*"){
					Eval.push(temp2*temp1);
				}
				else if(functionrpn[i].rpn[j].value=="/"){
					Eval.push(temp2/temp1);
				}
				else if(functionrpn[i].rpn[j].value=="^"){
					Eval.push(pow(temp2,temp1));
				}
			}
			else if(functionrpn[i].rpn[j].lexem=="FUN"){
				temp1=Eval.top();
				Eval.pop();
				if(functionrpn[i].rpn[j].value=="sin"){
					Eval.push(sin(temp1));
				}
				else if(functionrpn[i].rpn[j].value=="cos"){
					Eval.push(cos(temp1));
				}
				else if(functionrpn[i].rpn[j].value=="exp"){
					Eval.push(exp(temp1));
				}
				else if(functionrpn[i].rpn[j].value=="fabs"){
					Eval.push(fabs(temp1));
				}
				else if(functionrpn[i].rpn[j].value=="log"){
					Eval.push(log(temp1));
				}
				else if(functionrpn[i].rpn[j].value=="log10"){
					Eval.push(log10(temp1));
				}
			}
			else if(functionrpn[i].rpn[j].lexem=="VAR"){
				flag=true;
				for(int k=0;k<undefinedvar.size();k++){
					if(undefinedvar[k].variable==functionrpn[i].rpn[j].value){
						Eval.push(undefinedvar[k].value);
						flag=false;
						break;
					}
				}
				for(int k=0;k<i&&flag;k++){
					if(definedvar[k].variable==functionrpn[i].rpn[j].value){
						if(!definedvar[k].defined)	return NAN;
						Eval.push(definedvar[k].value);
						break;
					}
				}
			}
		}
		definedvar[i].value=Eval.top();
		definedvar[i].defined=true;
		Eval.pop();
	}
	result=definedvar[definedvar.size()-1].value;
	return result;
}
		


