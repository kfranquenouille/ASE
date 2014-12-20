CC = gcc

CFLAGS 	= -Wall -Werror -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g
CFLAGS += -m32

OBJ = switch_to.o

all: switch_to

switch_to: $(OBJ) switch_to.h
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	
.PHONY: clean realclean
clean:
	$(RM) $(OBJ)
realclean : clean
	$(RM) switch_to

