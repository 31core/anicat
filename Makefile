include src/Makefile
include lib/Makefile

CC = gcc
C_FLAGS = -Iinclude -c

all:$(objs)
	$(CC) $(objs) -o Anicat

%.o:%.c
	$(CC) $(C_FLAGS) $*.c -o $*.o

run:
	make all
	./Anicat test/test.ac > output.txt

release:
	make all
	strip Anicat

clean:
	rm $(objs)
	rm Anicat