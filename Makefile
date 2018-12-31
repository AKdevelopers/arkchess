# Makefile for programs to index and search an index.
FLAGS = -Wall -g -std=gnu99

ark : ark.o ray_attacks.o FEN_Parser.o
	gcc ${FLAGS} -o $@ $^ 

ark.o : ark.c ark.h
	gcc ${FLAGS} -c $<

ray_attacks.o : ray_attacks.c ray_attacks.h 
	gcc ${FLAGS} -c $<

FEN_Parser.o : FEN_Parser.c FEN_Parser.h 
	gcc ${FLAGS} -c $<

clean :
	-rm *.o ark 


