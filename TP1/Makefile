CC = gcc

CFLAGS 	= -Wall -Werror -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g
CFLAGS += -m32

OBJ = try.o try_mul.o

all: try_mul

try_mul: $(OBJ) try.h
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	
.PHONY: clean realclean
clean:
	$(RM) $(OBJ)
realclean : clean
	$(RM) try_mul

