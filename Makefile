CC = g++
CFLAGS = -std=c++11
LIBS = `pkg-config --cflags --libs opencv4`

all: juego

tablero: juego.cpp
	$(CC) -o juego juego.cpp $(CFLAGS) $(LIBS)

clean:
	rm -f juego