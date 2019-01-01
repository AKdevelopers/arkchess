FLAGS = -Wall -g -std=gnu99 

main : ark.o ray_attacks.o FEN_Parser.o
	gcc ${FLAGS} -o $@ main.c $^ 

ark.o : ark.c ark.h
	gcc ${FLAGS} -c $<

test.o : tests/test.c 
	gcc ${FLAGS} -c $<

ray_attacks.o : ray_attacks.c ray_attacks.h 
	gcc ${FLAGS} -c $<

FEN_Parser.o : FEN_Parser.c FEN_Parser.h 
	gcc ${FLAGS} -c $<

clean :
	-rm *.o main 


