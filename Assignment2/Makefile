
programName:=mykernel
CC:=gcc
CFLAGS:=-c
srcDir:=src
buildDir:=build
incDir:=inc

kernelDependencies:=$(srcDir)/kernel.c $(incDir)/kernel.h $(srcDir)/shell.c $(incDir)/shell.h $(srcDir)/pcb.c $(incDir)/pcb.h $(srcDir)/ram.c $(incDir)/ram.h 
shellDependencies:=$(srcDir)/shell.c $(srcDir)/interpreter.c $(srcDir)/shellmemory.c $(incDir)/shell.h $(incDir)/interpreter.h $(incDir)/shellmemory.h 

ifeq  ($(DEBUG), 1)
	CFLAGS+= -ggdb3
	programName=mysh_d
endif

all: $(programName)

$(programName): ./$(buildDir)/kernel.o ./$(buildDir)/shell.o ./$(buildDir)/interpreter.o ./$(buildDir)/shellmemory.o ./$(buildDir)/ram.o ./$(buildDir)/pcb.o
	$(CC) -o $(programName) ./$(buildDir)/kernel.o ./$(buildDir)/shell.o ./$(buildDir)/interpreter.o ./$(buildDir)/shellmemory.o ./$(buildDir)/ram.o ./$(buildDir)/pcb.o

# Shell.c plus any libraries used to build shell.c
./$(buildDir)/shell.o: $(shellDependencies)
	$(CC) $(CFLAGS) $(srcDir)/shell.c
	mv shell.o ./$(buildDir)/

# interpreter.c plus any libraries
./$(buildDir)/interpreter.o: $(srcDir)/interpreter.c $(incDir)/interpreter.h $(incDir)/shell.h
	$(CC) $(CFLAGS) $(srcDir)/interpreter.c
	mv interpreter.o ./$(buildDir)/

# interpreter.c plus any libraries
./$(buildDir)/shellmemory.o: $(srcDir)/shellmemory.c $(incDir)/shellmemory.h 
	$(CC) $(CFLAGS) $(srcDir)/shellmemory.c
	mv shellmemory.o ./$(buildDir)/

# cpu.c plus any libraries
./$(buildDir)/cpu.o: $(srcDir)/cpu.c $(incDir)/cpu.h
	$(CC) $(CFLAGS) $(srcDir)/cpu.c
	mv cpu.o ./$(buildDir)/

# kernel.c plus any libraries
./$(buildDir)/kernel.o: $(kernelDependencies) 
	$(CC) $(CFLAGS) $(srcDir)/kernel.c
	mv kernel.o ./$(buildDir)/

# pcb.c plus any libraries
./$(buildDir)/pcb.o: $(srcDir)/pcb.c $(incDir)/pcb.h
	$(CC) $(CFLAGS) $(srcDir)/pcb.c
	mv pcb.o ./$(buildDir)/

# ram.c plus any libraries
./$(buildDir)/ram.o: $(srcDir)/ram.c $(incDir)/ram.h
	$(CC) $(CFLAGS) $(srcDir)/ram.c
	mv ram.o ./$(buildDir)/

.PHONY: clean
clean:
	rm -f *.exe ./build/*.o mysh
