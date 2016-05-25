#include "dlfcn.h"
#include "fstream"
#include "iostream"
#include "cstdlib"
#include "ctime"
using namespace std;
typedef struct{
	long double *position,*pBest,*vel;

}Particles;
typedef struct {
	long double *variables,result;
	int paramcount;
	bool succes;
}Result;
Result optimize();
