CC := gcc
CFLAGS := -g -Wall
LDFLAGS := -L./
LIBS := -ldynlib

all: libdynlib.so app injection.o

libdynlib.so: dynlib.c
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@

app: app.c libdynlib.so
	$(CC) $(CFLAGS) $< $(LDFLAGS) $(LIBS) -o $@

injection.o: injection.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f libdynlib.so app injection.o
