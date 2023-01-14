CC:=cc
CFLAGS:=-g -Wall
CFLAGS+=-I headers -framework OpenGL -framework GLUT

NAME:=spacegame

SRC:= gameoflife.c
HEAD:=$(wildcard *.h)
OBJ:= $(SRC:.c=.o)

.PHONY: main clean run

main: $(OBJ) $(HEAD)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -rf *.o $(NAME)

run:
	./$(NAME)