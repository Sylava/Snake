CC = gcc
CFLAGS = -Wall -I include
LDFLAGS = -L lib -lmingw32 -lSDL2main -lSDL2

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=%.o)

Snake : $(OBJ)
	$(CC) $(OBJ) -o Snake $(LDFLAGS)

%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	del /Q *.o 2> NUL

fclean : clean
	del /Q Snake.exe 2> NUL

re :fclean Snake