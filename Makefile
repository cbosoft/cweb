CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LINK = 

OBJ = src/server.o \
			src/cache.o \
			src/hashtable.o \
			src/llist.o \
			src/error.o \
			src/util.o \
			src/request.o \
			src/io.o \
			src/mime.o \
			src/net.o

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) $< -o $@ -c

serv: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@
