CC=gcc

CFLAGS=-DSFML_STATIC -std=c++14 -g3 -pthread -IC:\libraries\SFML-2.4.2_gcc64\include 
LIBFLAGS=-LC:/libraries/SFML-2.4.2_gcc64/lib
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lstdc++

all:
	$(CC) -c $(CFLAGS) main.cpp -o main.o
	$(CC) $(LIBFLAGS) -o main.exe main.o geometry.o $(LDFLAGS)

	
pg:
	$(CC) $(CFLAGS) -pg main.cpp -o main.exe $(LIBFLAGS)
clean:
	rm -rf main.o main.exe