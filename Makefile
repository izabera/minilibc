CFLAGS = -march=native \
				 -Os \
				 -ffunction-sections \
				 -fdata-sections \
				 -fno-unwind-tables \
				 -fno-asynchronous-unwind-tables \
				 -s \
				 -fno-ident \
				 -Wall -Wextra

LDFLAGS = -Wl,--gc-sections -s

all: minilibc

minilibc: minilibc.c my_stdio.h.c my_string.h.c
	$(CC) $(CFLAGS) -c minilibc.c -o minilibc.o
	$(CC) $(LDFLAGS) minilibc.o -o minilibc
