
programName:=mysh
CC:=gcc
CFLAGS:=-c
srcDir:=src
buildDir:=build
incDir:=inc


ifeq  ($(DEBUG), 1)
	CFLAGS+= -ggdb3
	programName=mysh_d
endif

all: $(programName)

$(programName): ./$(buildDir)/shell.o ./$(buildDir)/interpreter.o ./$(buildDir)/shellmemory.o #./$(buildDir)/node.o 
	# To compile node, add ./$(buildDir)/node.o to end
	$(CC) -o $(programName) ./$(buildDir)/shell.o ./$(buildDir)/interpreter.o ./$(buildDir)/shellmemory.o 

# Shell.c plus any libraries used to build shell.c
./$(buildDir)/shell.o: $(srcDir)/shell.c $(srcDir)/interpreter.c $(srcDir)/shellmemory.c $(srcDir)/node.c $(incDir)/shell.h $(incDir)/interpreter.h $(incDir)/shellmemory.h $(incDir)/node.h 
	$(CC) $(CFLAGS) $(srcDir)/shell.c
	mv shell.o ./$(buildDir)/

# interpreter.c plus any libraries
./$(buildDir)/interpreter.o: $(srcDir)/interpreter.c $(incDir)/interpreter.h
	$(CC) $(CFLAGS) $(srcDir)/interpreter.c
	mv interpreter.o ./$(buildDir)/

# interpreter.c plus any libraries
./$(buildDir)/shellmemory.o: $(srcDir)/shellmemory.c $(incDir)/shellmemory.h 
	$(CC) $(CFLAGS) $(srcDir)/shellmemory.c
	mv shellmemory.o ./$(buildDir)/

# ./$(buildDir)/node.o: $(srcDir)/node.c $(incDir)/node.h
# 	$(CC) $(CFLAGS) $(srcDir)/node.c
# 	mv node.o ./$(buildDir)/

clean:
	rm -f *.exe ./build/*.o mysh
