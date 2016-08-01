all: minilibc

minilibc: minilibc.c
	$(CC) $(CFLAGS) $(LDFLAGS) minilibc.c -o minilibc
