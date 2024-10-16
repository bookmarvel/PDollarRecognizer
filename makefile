# This code was written completely by me (Asher Theys)
# No AI was used in the creation of this file

all: pdollar

main.o: main.cpp
	g++ main.cpp -g -c -o main.o

pdollar.o: pdollar.cpp
	g++ pdollar.cpp -g -c -o pdollar.o

pdollar: main.o pdollar.o
	g++ main.o pdollar.o -g -o pdollar

clean: pdollar
	./pdollar -r
	rm -f *.o pdollar