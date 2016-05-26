/*
This will check the input of the user from which will be on a text file,this will the make a c file which we will use later to compute the value 
of the objective funtion
for the time being i have created a temp.txt to test
still under construction..
*/
#include "chckfunfile.hpp"
vector<Variables> var;
vector<Line> lines;
int linenum=0;
Variables newVar(string temp,bool temp1){
	Variables temp2;
	temp2.variable=temp;
	temp2.haveValue=temp1;
	return temp2;
}
Line newLine(string temp,string temp1){
	Line temp2;
	temp2.variable=temp;
	temp2.line=temp1;
	return temp2;
}
bool checkline(string finalline){
	bool previousStr=false,previousDigit=false,previousOp=false,previousBrO=false,previousBrC=false,previousM=false,previouseq=false,previousMinus=false;
	string previous,variable;
	stack<char> bracket;
	int k;
	for(int i=0;i<finalline.length();i++){
		if(i==0){
			if(!isalpha(finalline[i])){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			previousStr=true;
			previous.push_back(finalline[i]);
			continue;
		}
		if(isalpha(finalline[i])||isdigit(finalline[i])){
			if(previousBrC==true){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			previousOp=false;
			previousBrO=false;
			//previous.append(finalline[i]);
			if(isalpha(finalline[i])){
				if(previousDigit){
					cout<<"Error at line "<<linenum<<endl;
					return false;
				}
				previousStr=true;
				previous.push_back(finalline[i]);
			}
			else if(previous.length()==0 && isdigit(finalline[i])){
				previousDigit=true;
				previous.push_back(finalline[i]);
			}
			else if(isdigit(finalline[i])){
				previous.push_back(finalline[i]);
			}
			previousMinus=false;
			//previousStr.append(finalline[i]);
		}
		else if(finalline[i]=='/'||finalline[i]=='*'||finalline[i]=='+'||finalline[i]=='-'){
			if(previousOp&&!previouseq&&finalline[i]!='-'){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			if(finalline[i]=='-'){
				previousMinus=true;
			}
			if(previousStr){
				for(k=0;k<var.size();k++){
					if(var[k].variable==previous){
						break;
					}
				}
				if(k==var.size()){
					var.push_back(newVar(previous,false));
						
				}
				previous.clear();	
			}
			//h=0;
			//sin(1);
			previousOp=true;
			previousDigit=false;
			previousStr=false;
			previousBrO=false;
			previousBrC=false;
			previous.clear();
			continue;
		}
		else if(finalline[i]=='.'){
			if(!previousDigit){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}	
		}
		else if(finalline[i]=='='){
			if(previousOp||previousBrO||previousBrC||previousDigit){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			//cout<<"this";
			var.push_back(newVar(previous,true));
			variable=previous;
			previous.clear();
			previousOp=true;
			previousDigit=false;
			previousStr=false;
			previouseq=true;
		}
		/*else if(finalline[i]=='^'){
			if(!previousBrC){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			
		}*/
		else if(i==finalline.length()-1&&finalline[i]==';'){
			if(previousOp||previousBrO){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			else if(previousStr){
				for(k=0;k<var.size();k++){
					if(var[k].variable==previous){
						break;
					}
				}
				if(k==var.size()){
					var.push_back(newVar(previous,false));
							
				}
				previous.clear();	
				continue;
			}
			previous.clear();
			continue;
		}
		else if(finalline[i]=='('){
			if(previousDigit||previousBrC){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			else if(previousStr){
				if(previous=="exp"||previous=="log"||previous=="sin"||previous=="log10"||previous=="sin"||previous=="cos"||previous=="fabs"){
					//previousM=true
					previous.clear();
				}
				else{
					cout<<"Error at line "<<linenum<<endl;
					return false;
				}
			}
			bracket.push('(');
			previousOp=false;
			previousDigit=false;
			previousStr=false;
			previousBrO=true;
		}
		else if(finalline[i]==')'){
			if(previousOp||previousBrO){
				cout<<"Error at line "<<linenum<<endl;
				return false;
			}
			if(previousStr){
				for(k=0;k<var.size();k++){
					if(var[k].variable==previous){
						break;
					}
				}
				if(k==var.size()){
					var.push_back(newVar(previous,false));
							
				}
				previous.clear();	
			}
			bracket.pop();
			previousOp=false;
			previousDigit=false;
			previousStr=false;
			previousBrC=true;
		}
		else{
			cout<<"Error at line "<<finalline<<endl;
			return false;
		}
	}
	if(!bracket.empty()){
		cout<<"Error at line "<<finalline<<endl;
		return false;
	}
	if(!previouseq){
		cout<<"Error at line "<<finalline<<endl;
		return false;
	}
	lines.push_back(newLine(variable,finalline));
	return true;
}
bool checkfile(string filename){
	const char *c=filename.c_str();
	ifstream inputfile(c);
	ofstream datafile("data.txt");
	string line,finalline,choice;
	int paramcount=0;
	bool mainloopflag=true;
	if(!inputfile.good()){
		cout<<"objective file could not be loaded"<<endl;
		return false;
	}
	while(mainloopflag){
		//line=new string;
		linenum++;
		getline(inputfile,line);
		if(inputfile.eof()){
			mainloopflag=false;
		}
		if(line[line.length()-1]!=';'){
			finalline.append(line);
			continue;
		}
		finalline.append(line);
		if(checkline(finalline)){
			//cout<<finalline<<endl;
		}
		
		finalline.clear();
	}
	/*for(int i=0;i<var.size();i++){
		cout<<var[i].variable<<' '<<var[i].haveValue<<endl;
	}*/
	/*for(int i=0;i<lines.size();i++){
		cout<<lines[i].variable<<' '<<lines[i].line<<endl;
	}*/
	for(int i=0;i<var.size();i++){
		if(!var[i].haveValue){
			cout<<var[i].variable<<" is not defined. Is it a parameter? press y is yes: ";
			cin>>choice;
			if(choice=="y"){
				datafile<<var[i].variable<<endl;
				paramcount++;
			}
			else{
				cout<<"please define "<<var[i].variable<<endl;
				return false;
			}
		}
	}
	if(paramcount==0){
		cout<<"all variables are defined"<<endl;
		return false;
	}
	datafile<<'#'<<endl;
	inputfile.close();
	datafile.close();
	return true;
}
bool createCfile(){
	ofstream finalfile("obj.cpp");
	ifstream datafile("data.txt");
	string variables;
	int variablecount=0;
	finalfile<<"extern \"C\" long double obj(long double *arr);"<<endl<<"#include \"cmath\""<<endl<<"long double obj(long double *arr){"<<endl<<"long double ";
	//cout<<lines.size();
	for(int i=0;i<lines.size();i++){
		finalfile<<lines[i].variable;
		if(i==lines.size()-1) finalfile<<';';
		else	finalfile<<',';
	}
	finalfile<<endl;
	datafile>>variables;
	while(variables!="#"&&!datafile.eof()){
		finalfile<<"long double "<<variables<<"=arr["<<variablecount++<<"];"<<endl;
		datafile>>variables;
	}
	for(int i=0;i<lines.size();i++){
		finalfile<<lines[i].line<<endl;
	}
	finalfile<<"return "<<lines[lines.size()-1].variable<<";}";
	finalfile.close();
	var.clear();
	lines.clear();
	return true;
}

