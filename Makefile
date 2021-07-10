include src/Makefile

C_FLAGS = -Iinclude

all:$(objs)
	gcc $(objs) -o Anicat

%.o:%.c
	gcc $(C_FLAGS) -c $*.c -o $*.o
run:
	make all
	./Anicat
clean:
	rm $(objs)