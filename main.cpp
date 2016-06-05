#include "iostream"
#include "optimizer.hpp"
#include "parser.hpp"
using namespace std;
int main(){
	MFparser parser("fun.txt");
	Optimizer opti;
	Result result;
	parser.MFlexicalanalzye();
	parser.MFsyntaxanalysis();
	parser.MFsemanticanalysis();
	//parser.debugprintL();
	if(!parser.bad()){
		vector<vector<Lexem> > Lout=parser.MFgetparseroutput();
		opti.init(Lout);
		opti.optimizePSO(false);
		result=opti.getresult();
		cout<<"optimal= "<<result.optimalvalue<<endl;
		for(int i=0;i<result.variable.size();i++){
			cout<<result.variable[i]<<"= "<<result.optimalvalues[i]<<endl;
		}
	}
	return 0;
}
