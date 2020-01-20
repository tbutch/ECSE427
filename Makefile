
programName:=mysh
CC:=gcc
CFLAGS=-c

ifeq  ($(DEBUG), 1)
	CFLAGS+= -ggdb3
	programName=mysh_d
endif

all: $(programName)

$(programName): shell.o interpreter.o shellmemory.o node.o 
	$(CC) -o $(programName) shell.o interpreter.o shellmemory.o node.o

# Shell.c plus any libraries used to build shell.c
shell.o: shell.c shell.h interpreter.h shellmemory.h interpreter.c shellmemory.c node.h node.c
	$(CC) $(CFLAGS) shell.c

# interpreter.c plus any libraries
interpreter.o: interpreter.c interpreter.h
	$(CC) $(CFLAGS) interpreter.c 

# interpreter.c plus any libraries
shellmemory.o: shellmemory.c shellmemory.h 
	$(CC) $(CFLAGS) shellmemory.c

node.o: node.c node.h
	$(CC) $(CFLAGS) node.c 

clean:
	rm -f shell.o shellmemory.o interpreter.o $(programName).exe $(programName) a.exe node.o
