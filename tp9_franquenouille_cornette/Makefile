# $Id: Makefile,v 1.1 2002/12/06 14:25:18 marquet Exp $
##############################################################################

ROOTDIR=.

CC	= gcc
CFLAGS	= -Wall -ansi -pedantic 
CFLAGS  += -g -m32
LIBDIR  = -L$(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -lhardware

###------------------------------
### Main targets 
###------------------------------------------------------------
BINARIES= mi_user
###OBJECTS	= $(addsuffix .o,\
###	  drive mbr vol)
OBJECTS	= $(addsuffix .o,\
	   mi_kernel)

all: $(BINARIES) $(OBJECTS)

###------------------------------
### Binaries
###------------------------------------------------------------

mi_user: mi_user.o mi_kernel.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBDIR) $(LIBS)



###------------------------------
### #include dependences 
###------------------------------------------------------------
# you may fill these lines with "make depend"
mi_user.o: mi_user.c mi_kernel.c mi_syscall.h
mi_kernel.o: mi_kernel.c mi_syscall.h
%.o: %.c
	$(CC) $(CFLAGS) -c $< $(INCDIR)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES) 
realclean: clean 
	$(RM) vdiskA.bin vdiskB.bin
depend : 
	$(CC) -MM $(INCDIR) *.c

