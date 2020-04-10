# Assignment 3: mykernel

## Introduction
This Kernel was created by Tristan Bouchard in January of 2020 and developed over the Winter 2020 semester. It currently supports the commands specified when running the "help" command. The source code can be compiled on Windows using the supplied Makefile. IT DOES NOT RUN CORRECTLY ON LINUX, it is now tethered to Windows due to the format of the system() calls used. It can also be compiled with debugging symbols using the `make DEBUG=1` flag, compiles the code with the required symbols to debug with gdb.

The resulting executable, `mykernel.exe` or `mykernel` depending on the machine it was compiled on, can then be run on its own:

> `mykernel.exe` or `./mykernel`

 or with a well-formed scripts as standard input,

> `mykernel.exe < wellFormedScript.txt`

or
>`./mykernel < wellFormedScript.txt`

See the [Script assumptions](#scriptAssumptions) section below for more information on well-formed scripts.

To find the textbook questions section of this assignment, see [Assignment 3 Part 1](/Assignment3_Part_1.md)

## Exec Command
This shell is capable of running up to 3 well formed scripts. The script names must not necessarily all be different, there may be duplicates. If the specified programs do not fit in RAM completely, the RAM is flushed and no script is executed. The maximum script size at the moment is limited by the RAM size, which corresponds to 40 lines of code maximum.

## Setting Variables
This shell is capable of storing up to SHELL_MEMORY_SIZE variables, which are case sensitive. Only the first entry after the variable will be taken into account to set the variable. For example, the command:

> `set bob Montreal Canadiens`

will set the variable `bob` to `Montreal`. Trying `print Bob` (with an uppercase 'b') will result in a `Variable does not exist` message, only `print bob` (all in lowercase, as in the command) will print `Montreal`

## Size assumptions
This assignment makes the following assumptions about the input size and restrics the shell memory in the following way:

- Shell memory is limited to 100 variables, but can be modified by changing the SHELL_MEMORY_SIZE variable in "shell.h"
- Input size is limited to 1000 characters, but can be modified by changing the INPUT_SIZE variable in "shell.h"
- Ram memory size is limited to 1000 lines, but can be modified by changing the RAMSIZE variable in "ram.h"
- Base CPU quanta is limited to 2 instrutions, but can be modified by changing the BASE_QUANTA variable in "cpu.h"
- RAM memory size is set in the RAM.h header file. It is possible to modify the current frame sizes and number of frames, currently set under FRAME_SIZE = 4 and NUMBER_OF_FRAMES = 10, which limits the size of the RAM to 40 lines of code.

## Script assumptions<a name="scriptAssumptions"></a>
This shell also assumes certain specifications, based on information gathered on Piazza and through questions with the professor:
- A well formed script can contain newlines.
- A well formed script continues execution of the script until the quit command is hit, even if a malformed command is found in the script.
- A script containing recursion must be let to recurse infinitely, similarly to what the bash shell does. At one point, the "fopen" function returns null and cannot open the file, which ends the recursion.
- A script does not need to be terminated by the ".txt" extension, the filename can be anything.
- Commands are case sensitive.
- Commands can contain more than one space between keywords.
