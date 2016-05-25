all: main
main: main.o funfile.o optimize.o
	g++ main.o funfile.o optimize.o -o main -ldl

main.o: main.cpp
	g++ -c main.cpp
funfile.o: chckfunfile.cpp
	g++ -c -o funfile.o chckfunfile.cpp
optimize.o: optimizer.cpp
	g++ -c -o optimize.o optimizer.cpp
clean:
	rm *o
