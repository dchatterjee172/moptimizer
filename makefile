all: main
main: main.o parser.o optimize.o eval.o
	g++ main.o parser.o eval.o optimize.o -o main

main.o: main.cpp
	g++ -g -c main.cpp
parser.o: parser.cpp
	g++ -g -std=c++11 -c -o parser.o parser.cpp

eval.o: evaluator.cpp
	g++ -g -std=c++11 -c -o eval.o evaluator.cpp

optimize.o: optimizer.cpp
	g++ -g -c -o optimize.o optimizer.cpp
clean:
	rm *o
