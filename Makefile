CC = musl-gcc
CFLAGS = -march=native \
				 -Os \
				 -ffunction-sections \
				 -fdata-sections \
				 -fno-unwind-tables \
				 -fno-asynchronous-unwind-tables \
				 -s \
				 -fno-ident \
				 -nostdinc \
				 -ffreestanding \
				 -Wall -Wextra

LDFLAGS = -Wl,--gc-sections -s -static -nostdlib -nodefaultlibs

all: minilibc

stdio:
	$(CC) $(CFLAGS) -c stdio/stdio.c -o obj/stdio.o

minilibc: minilibc.c prototypes.h my_*.c Makefile
	$(CC) $(CFLAGS) -c minilibc.c -o minilibc.o
	$(CC) $(LDFLAGS) minilibc.o -o minilibc

prototypes.h: my_*.c
	./proto > prototypes.h
