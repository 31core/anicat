include src/Makefile

CC = gcc
C_FLAGS = -Iinclude

all:$(objs)
	$(CC) $(objs) -o Anicat

%.o:%.c
	$(CC) $(C_FLAGS) -c $*.c -o $*.o
test:
	make all
	./Anicat
clean:
	rm $(objs)
	rm Anicat