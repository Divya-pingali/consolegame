FLAGS = -pedantic-errors -std=c++11

game.o: game.cpp game.h popup.h
	g++ $(FLAGS) -c game.cpp

popup.o: popup.cpp popup.h game.h
	g++ $(FLAGS) -c popup.cpp

main.o: main.cpp game.h popup.h
	g++ $(FLAGS) -c main.cpp

main: main.o game.o popup.o
	g++ $(FLAGS) main.o game.o popup.o -o main -lncurses

clean:
	rm -f game.o popup.o main.o main

.PHONY: clean