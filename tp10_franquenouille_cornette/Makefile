CC=gcc
CFLAGS=-W -Wall -pedantic -m32 -I ./include
LDFLAGS=-W -Wall -pedantic -m32 -L ./lib -lhardware
EXEC=kernel

all: $(EXEC)

kernel: mi_kernel.c swap.o mi_user.o mi_handler.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean realclean

clean:
	rm -rf *.o *~

realclean: clean
	rm -rf $(EXEC)
