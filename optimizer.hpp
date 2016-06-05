#ifndef OPTIMIZE
#define OPTIMIZE
#include "cstdlib"
#include "ctime"
#include "evaluator.hpp"
struct particlepso{
	std::vector<long double> position,pBest,vel;
	particlepso(std::vector<long double> p,std::vector<long double> pb,std::vector<long double> v){
		position=p;
		pBest=pb;
		vel=v;
	}
};
typedef struct particlepso ParticlePSO;
typedef struct{
	std::vector<long double> optimalvalues;
	long double optimalvalue;
	std::vector<std::string> variable;
}Result;
class Optimizer{
	Result result;
	MFfunction function;
	std::vector<ParticlePSO> initPSOparticles(int count);
	std::vector<long double> getGbestPSO(std::vector<ParticlePSO>,bool);
	public:
		void init(std::vector<std::vector<Lexem> >);
		void optimizePSO(bool,int particlenum=30,int iteration=1000,float c1=2,float c2=2.5,float w=.55,float contriction=.8);
		Result getresult();
};
#endif
