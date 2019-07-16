CC = gcc
CFLAGS = -Wall -Wextra -O2
LINK = 

OBJ = src/server.o \
			src/cache.o \
			src/file.o \
			src/hashtable.o \
			src/llist.o \
			src/mime.o \
			src/net.o

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) $< -o $@ -c

serv: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@
