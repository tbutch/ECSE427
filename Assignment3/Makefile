
programName:=mykernel
CC:=gcc
CFLAGS:=-c
srcDir:=src
buildDir:=build
incDir:=inc

kernelDependencies:=$(srcDir)/kernel.c $(incDir)/kernel.h $(srcDir)/shell.c $(incDir)/shell.h $(srcDir)/pcb.c $(incDir)/pcb.h \
					$(srcDir)/ram.c $(incDir)/ram.h $(srcDir)/stringUtilities.c $(incDir)/stringUtilities.h \
					$(srcDir)/memorymanager.c $(incDir)/memorymanager.h $(srcDir)/frameQueue.c $(incDir)/frameQueue.h
shellDependencies:=$(srcDir)/shell.c $(srcDir)/interpreter.c $(srcDir)/shellmemory.c $(incDir)/shell.h $(incDir)/interpreter.h \
				   $(incDir)/shellmemory.h 
ramDependencies:=$(srcDir)/ram.c $(incDir)/ram.h $(srcDir)/shell.c $(incDir)/shell.h $(srcDir)/stringUtilities.c $(incDir)/stringUtilities.h \
				 $(srcDir)/pcb.c $(incDir)/pcb.h
cpuDependencies:=$(srcDir)/cpu.c $(incDir)/cpu.h $(srcDir)/ram.c $(incDir)/ram.h $(srcDir)/interpreter.c $(incDir)/interpreter.h \
				 $(srcDir)/shellmemory.c $(incDir)/shellmemory.h
interpreterDependencies:=$(srcDir)/interpreter.c $(incDir)/interpreter.h $(srcDir)/shell.c $(incDir)/shell.h $(srcDir)/ram.c $(incDir)/ram.h \
						 $(srcDir)/pcb.c $(incDir)/pcb.h $(srcDir)/cpu.c $(incDir)/cpu.h $(srcDir)/stringUtilities.c $(incDir)/stringUtilities.h \
						 $(srcDir)/shellmemory.c $(incDir)/shellmemory.h
pcbDependencies:=$(srcDir)/pcb.c $(incDir)/pcb.h
shellmemoryDependencies:=$(srcDir)/shellmemory.c $(incDir)/shellmemory.h $(srcDir)/interpreter.c $(incDir)/interpreter.h \
						 $(srcDir)/stringUtilities.c $(incDir)/stringUtilities.h
stringUtilitiesDependencies:=$(srcDir)/stringUtilities.c $(incDir)/stringUtilities.h
memoryManagerDependencies:=$(srcDir)/memorymanager.c $(incDir)/memorymanager.h $(srcDir)/pcb.c $(incDir)/pcb.h $(srcDir)/kernel.c $(incDir)/kernel.h \
						   $(srcDir)/interpreter.c $(incDir)/interpreter.h $(srcDir)/stringUtilities.c $(incDir)/stringUtilities.h \
						   $(srcDir)/shell.c $(incDir)/shell.h $(srcDir)/ram.c $(incDir)/ram.h $(srcDir)/frameQueue.c $(incDir)/frameQueue.h
frameQueueDependencies:=$(srcDir)/frameQueue.c $(incDir)/frameQueue.h

ifeq  ($(DEBUG), 1)
	CFLAGS+= -ggdb3
	programName=mykernel_d
endif

all: $(programName)

$(programName): ./$(buildDir)/kernel.o ./$(buildDir)/shell.o ./$(buildDir)/interpreter.o ./$(buildDir)/shellmemory.o ./$(buildDir)/ram.o ./$(buildDir)/pcb.o ./$(buildDir)/stringUtilities.o ./$(buildDir)/cpu.o ./$(buildDir)/memorymanager.o ./$(buildDir)/frameQueue.o
	$(CC) -o $(programName) ./$(buildDir)/kernel.o ./$(buildDir)/shell.o ./$(buildDir)/interpreter.o ./$(buildDir)/shellmemory.o ./$(buildDir)/ram.o ./$(buildDir)/pcb.o ./$(buildDir)/stringUtilities.o ./$(buildDir)/cpu.o ./$(buildDir)/memorymanager.o ./$(buildDir)/frameQueue.o

# Shell.c plus any libraries used to build shell.c
./$(buildDir)/shell.o: $(shellDependencies)
	$(CC) $(CFLAGS) $(srcDir)/shell.c
	mv shell.o ./$(buildDir)/

# interpreter.c plus any libraries
./$(buildDir)/interpreter.o: $(interpreterDependencies)
	$(CC) $(CFLAGS) $(srcDir)/interpreter.c
	mv interpreter.o ./$(buildDir)/

# interpreter.c plus any libraries
./$(buildDir)/shellmemory.o: $(shellmemoryDependencies)
	$(CC) $(CFLAGS) $(srcDir)/shellmemory.c
	mv shellmemory.o ./$(buildDir)/

# cpu.c plus any libraries
./$(buildDir)/cpu.o: $(cpuDependencies)
	$(CC) $(CFLAGS) $(srcDir)/cpu.c
	mv cpu.o ./$(buildDir)/

# kernel.c plus any libraries
./$(buildDir)/kernel.o: $(kernelDependencies) 
	$(CC) $(CFLAGS) $(srcDir)/kernel.c
	mv kernel.o ./$(buildDir)/

# pcb.c plus any libraries
./$(buildDir)/pcb.o: $(pcbDependencies)
	$(CC) $(CFLAGS) $(srcDir)/pcb.c
	mv pcb.o ./$(buildDir)/

# ram.c plus any libraries
./$(buildDir)/ram.o: $(ramDependencies)
	$(CC) $(CFLAGS) $(srcDir)/ram.c
	mv ram.o ./$(buildDir)/

./$(buildDir)/stringUtilities.o: $(stringUtilitiesDependencies)
	$(CC) $(CFLAGS) $(srcDir)/stringUtilities.c
	mv stringUtilities.o ./$(buildDir)/

./$(buildDir)/memorymanager.o: $(memoryManagerDependencies)
	$(CC) $(CFLAGS) $(srcDir)/memorymanager.c
	mv memorymanager.o ./$(buildDir)/

./$(buildDir)/frameQueue.o: $(frameQueueDependencies)
	$(CC) $(CFLAGS) $(srcDir)/frameQueue.c
	mv frameQueue.o ./$(buildDir)/

.PHONY: clean
clean:
	rm -f *.exe ./build/*.o mysh mysh_d mykernel mykernel_d
