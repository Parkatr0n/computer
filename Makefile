
LIBS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system

build:
	@g++ main.cpp -o bin/computer $(LIBS)
	@bin/computer

debug:
	g++ main.cpp -g -o bin/computer $(LIBS)
	gdb

