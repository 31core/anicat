include AST/Makefile
include Program/Makefile
include Parser/Makefile

C_FLAGS = -Iinclude

all:$(ast_obj) $(program_obj)
	gcc $(ast_obj) $(program_obj) -o Anicat

%.o:%.c
	gcc $(C_FLAGS) -c $*.c -o $*.o
run:
	make all
	./Anicat
clean:
	rm $(ast_obj) $(program_obj)