#include "optimizer.hpp"
long double *gBest;
void *handle,*function;
char *error;
long double (*obj)(long double *);
Result finalResult;
bool sharedObject(){
	handle=dlopen("./obj.so",RTLD_NOW);
	if(!handle){
		fputs(dlerror(),stderr);
		return false;
	}
	function=dlsym(handle,"obj");
	if((error=dlerror())!=NULL){
		fputs(error,stderr);
		return false;
	}
	obj=(long double (*)(long double *))function;
	return true;
}
int getParamcount(){
	ifstream datafile("data.txt");
	int paramcount=0;
	string parameter;
	datafile>>parameter;
	while(parameter!="#"){
		paramcount++;
		datafile>>parameter;
	}
	datafile.close();
	return paramcount;
}
Particles initParticle(int paramcount){
	Particles temp;
	temp.position=new long double[paramcount];
	temp.pBest=new long double[paramcount];
	temp.vel=new long double[paramcount];
	for(int i=0;i<paramcount;i++){
		temp.vel[i]=0;
		do{
			for(int i=0;i<paramcount;i++){
				temp.position[i]=0+(rand()/(float)RAND_MAX)*100;
				temp.pBest[i]=temp.position[i];
			}
		}while(0);
	}
	return temp;
}
long double *getGbest(Particles *particles,int particlecount,int maxorminF){
	long double *maxormin=particles[0].pBest;
	for(int i=1;i<particlecount;i++){
		if(maxorminF!=0&&(*obj)(maxormin)<(*obj)(particles[i].pBest))
			maxormin=particles[i].pBest;
		else if(maxorminF==0&&(*obj)(maxormin)>(*obj)(particles[i].pBest))
			maxormin=particles[i].pBest;
	}
	return maxormin;
}
void psoBeg(int iteration,int particlecount,int paramCount,float alpha1,float alpha2,float w,float constriction,int maxormin){// if maxormin=0 then minimization
	Particles particles[particlecount];
	int it=0;
	for(int i=0;i<particlecount;i++){
		particles[i]=initParticle(paramCount);
	}
	gBest=getGbest(particles,particlecount,maxormin);
	do{
		for(int i=0;i<particlecount;i++){
			for (int j=0;j<paramCount;j++){
				particles[i].vel[j]=(w*particles[i].vel[j]+alpha1*(0+rand()/(float)RAND_MAX)*(particles[i].pBest[j]-particles[i].position[j])+alpha2*(0+rand()/(float)RAND_MAX)*(gBest[j]-particles[i].position[j]))*constriction;
				particles[i].position[j]+=particles[i].vel[j];
			}
			if(maxormin!=0&&(*obj)(particles[i].position)>(*obj)(particles[i].pBest)){
				particles[i].pBest=particles[i].position;
			}
			else if(maxormin==0&&(*obj)(particles[i].position)<(*obj)(particles[i].pBest)){
				particles[i].pBest=particles[i].position;
			}
		}
		gBest=getGbest(particles,particlecount,maxormin);
		it++;
		//printf("%llf\n%llf\n\n",(*obj)(gBest),gBest[0]);
	}while(it<=iteration);
	//cout<<(*obj)(gBest)<<endl<<gBest[0]<<" "<<gBest[1]<<endl;
	//return gBest;
	finalResult.result=(*obj)(gBest);
	finalResult.variables=gBest;
	finalResult.paramcount=paramCount;
}

Result optimize(){
	srand(time(NULL));
	int paramcount=getParamcount();
	finalResult.succes=true;
	if(!sharedObject()){
		cout<<"problem loading objective file"<<endl;
		finalResult.succes=false;
		return finalResult;
	}
	psoBeg(300,20,paramcount,2.1,2.6,.56,.76,0);
	return finalResult;
}
