FLAGS = -Wall -g -std=gnu99 
TYPES = types.h

main : move_gen.o ray_attacks.o FEN_Parser.o
	gcc ${FLAGS} -o $@ main.c $^ 

move_gen.o : move_gen.c move_gen.h
	gcc ${FLAGS} -c $<

test.o : tests/test.c 
	gcc ${FLAGS} -c $<

ray_attacks.o : ray_attacks.c ray_attacks.h 
	gcc ${FLAGS} -c $<

FEN_Parser.o : FEN_Parser.c FEN_Parser.h 
	gcc ${FLAGS} -c $<

clean :
	-rm *.o main 


