#include "fstream"
#include "stack"
#include "cctype"
#include "iostream"
#include "vector"
#include "cstdlib"
using namespace std;
typedef struct{
	string variable;
	bool haveValue;
}Variables;
typedef struct{
	string variable;
	string line;
}Line;
bool createCfile();
bool checkfile();
bool checkline();
