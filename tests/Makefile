FLAGS = -Wall -g -std=gnu99 
SRC_DIR = ../src
SRC_FILES = $(SRC_DIR)/move_gen.c $(SRC_DIR)/ray_attacks.c $(SRC_DIR)/FEN_Parser.c $(SRC_DIR)/types.c

test : test.c $(SRC_FILES)
	gcc $(FLAGS) -o $@ $^ -lcunit

clean :
	-rm *.o test 

