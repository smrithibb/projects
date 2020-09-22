make all: gitint-shell

gitint-shell: gitint.o gitint-shell.cpp
	g++ -g -Wall --std=c++11 gitint-shell.cpp gitint.o -o gitint-shell

gitint.o: gitint.cpp gitint.h
	g++ -g -Wall --std=c++11 -c gitint.cpp -o gitint.o

clean:
	rm -f *.o gitint-shell *.o *~

*PHONY: clean


