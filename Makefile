include src/Makefile
include lib/Makefile

CC = gcc
C_FLAGS = -Iinclude -c

all:$(objs)
	$(CC) $(objs) -o Anicat

%.o:%.c
	$(CC) $(C_FLAGS) $*.c -o $*.o

check:
	make all
	./Anicat test/test.ac

release:
	make all
	strip Anicat

clean:
	rm $(objs)
	rm Anicat