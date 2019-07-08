CC=gcc
CFLAGS=-I.
DEPS = shopping_list.h
OBJ = shopping_list.o

%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

shopping_list: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS)
