#include "chckfunfile.hpp"
#include "optimizer.hpp"
int main(){
	bool mainloopflag=true,fileload=false;
	string command,variable;
	ifstream helpfile("help.txt"),datafile("data.txt");
	Result finalResult;
	/*while(!helpfile.eof()){
		cout<<helpfile;
	}*/
	while(mainloopflag){
		cout<<">>> ";
		cin>>command;
		if(command=="objective"){
			if(checkfile()&&createCfile()){
				fileload=true;
				system("g++ -shared -fPIC -o obj.so obj.cpp");
			}
			else{
				cout<<"There was an error"<<endl;
			}
		}
		else if(command=="optimize"){
			if(fileload){
				finalResult=optimize();
				if(finalResult.succes==false){
					cout<<"There was an error"<<endl;
				}
				else{
					for(int i=0;i<finalResult.paramcount;i++){
						datafile>>variable;
						cout<<variable<<"= "<<finalResult.variables[i]<<endl;
					}
					cout<<"Result= "<<finalResult.result<<endl;
					datafile.seekg(0,datafile.beg);
				}
			}
			else{
				cout<<"objective funtion is not loaded"<<endl;
			}
		}
		else if(command=="quit"){
			cout<<"good bye"<<endl;
			return 0;
		}
		
		else{
			cout<<"sorry i can't understand you.enter \"objective\" to load objective funtion file,\"optimize\" to optimize the objective funtion,\"quit\" to exit"<<endl;
		}
	}
	return 0;
}
