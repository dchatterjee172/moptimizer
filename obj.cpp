extern "C" long double obj(long double *arr);
#include "cmath"
long double obj(long double *arr){
long double a,b,OBJ;
long double x=arr[0];
long double y=arr[1];
a=x-3.14159;
b=y-3.14159;
OBJ=-cos(x)*cos(y)*exp(-(a*a+b*b));
return OBJ;}