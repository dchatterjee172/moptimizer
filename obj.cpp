extern "C" long double obj(long double *arr);
#include "cmath"
long double obj(long double *arr){
long double a,b,c,OBJ;
long double x=arr[0];
long double y=arr[1];
a=1.5-x+x*y;
b=2.25-x+x*y*y;
c=2.625-x+x*y*y*y;
OBJ=a*a+b*b+c*c+2;
return OBJ;}