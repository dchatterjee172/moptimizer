#include "fstream"
#include "stack"
#include "cctype"
#include "iostream"
#include "vector"
using namespace std;
typedef struct{
	string variable;
	bool haveValue;
}Variables;
typedef struct{
	string variable;
	string line;
}Line;
