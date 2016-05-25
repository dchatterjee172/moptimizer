#include "dlfcn.h"
#include "stdio.h"
#include "fstream"
#include "iostream"
#include "cstdlib"
#include "ctime"
#include <iomanip>
#include "stdio.h"
using namespace std;
typedef struct{
	long double *position,*pBest,*vel;

}Particles;
long double *gBest;
void *handle,*function;
char *error;
long double (*obj)(long double *);
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
long double * psoBeg(int iteration,int particlecount,int paramCount,float alpha1,float alpha2,float w,float constriction,int maxormin){// if maxormin=0 then minimization
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
	cout<<(*obj)(gBest)<<endl;
	return gBest;
}

int main(){
	srand(time(NULL));
	int paramcount=getParamcount();
	sharedObject();
	long double *result=psoBeg(300,15,paramcount,1,1.2,.5,.8,0);
}
