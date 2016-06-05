#include "optimizer.hpp"
using namespace std;
void Optimizer::init(vector<vector<Lexem> > Lout){
	function.MFgetparseroutput(Lout);
	result.variable=function.MFundefinedvar();
	//for(int i=0;i<result.variable.size();i++)	cout<<result.variable[i];
	srand(time(NULL));
}

vector<ParticlePSO> Optimizer::initPSOparticles(int count){
	vector<ParticlePSO> particles;
	vector<long double> position,pBest,vel;
	while(count--){
		for(int i=0;i<result.variable.size();i++){
			position.push_back(-100+rand()/(float)RAND_MAX*200);
			vel.push_back(0);
			pBest.push_back(position[i]);
		}
		particles.push_back(ParticlePSO(position,pBest,vel));
		position.clear();
		pBest.clear();
		vel.clear();
	}
	return particles;
}

vector<long double> Optimizer::getGbestPSO(vector<ParticlePSO> particles,bool maximize){
	vector<long double> temp=particles[0].pBest;
	for(int i=0;i<particles.size();i++){
		if((maximize&&function.MFevaluate(temp)<function.MFevaluate(particles[i].pBest))||(!maximize&&function.MFevaluate(temp)>function.MFevaluate(particles[i].pBest))){
			temp=particles[i].pBest;
		}
	}
	return temp;
}

void Optimizer::optimizePSO(bool maximize,int particlenum,int iteration,float c1,float c2,float w,float constriction){
	vector<ParticlePSO> particles=initPSOparticles(particlenum);
	vector<long double> Gbest=getGbestPSO(particles,maximize);
	
	do{
		for(int i=0;i<particles.size();i++){
			
			for(int j=0;j<result.variable.size();j++){
				particles[i].vel[j]=(w*particles[i].vel[j]+rand()/(float)RAND_MAX*c1*(particles[i].pBest[j]-particles[i].position[j])+rand()/(float)RAND_MAX*c2*(Gbest[j]-particles[i].position[j]))*constriction;
				particles[i].position[j]+=particles[i].vel[j];
			}
			
			if((maximize&&function.MFevaluate(particles[i].position)>function.MFevaluate(particles[i].pBest))||(!maximize&&function.MFevaluate(particles[i].position)<function.MFevaluate(particles[i].pBest))){
			particles[i].pBest=particles[i].position;
		}
		
		}
		Gbest=getGbestPSO(particles,maximize);
	}while(iteration--);
	result.optimalvalues=Gbest;
	result.optimalvalue=function.MFevaluate(Gbest);
}
Result Optimizer::getresult(){
	return result;
}
