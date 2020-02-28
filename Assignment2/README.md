# Assignment 2: mykernel

## Introduction
This Kernel was created by Tristan Bouchard in January of 2020 and submitted January 29th 2020. It currently supports the commands specified when running the "help" command. The source code can be compiled both on Windows using the supplied Makefile. IT DOES NOT RUN CORRECTLY ON LINUX for some reason I cannot figure out, just run it on Windows. It can also be compiled with debugging symbols using the `make DEBUG=1` flag, which enables the required symbols to debug with gdb.

The resulting executable, `mykernel.exe` or `mykernel` depending on the machine it was compiled on, can then be run on its own:

> `mykernel.exe` or `./mykernel`

 or with a well-formed scripts as standard input,

> `mykernel.exe < wellFormedScript.txt`

or
>`./mykernel < wellFormedScript.txt`

See the [Script assumptions](#scriptAssumptions) section below for more information on well-formed scripts.

## Exec Command
This shell is capable of running up to 3 well formed scripts. The scripts must all be different, there cannot be duplicates. If duplicates are detected, no program execution occurs. Likewise, if the specified programs do not fit in RAM completely, the RAM is flushed and no script is executed.

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

## Script assumptions<a name="scriptAssumptions"></a>
This shell also assumes certain specifications, based on information gathered on Piazza and through questions with the professor:
- A well formed script must end with a "quit" command.
- A well formed script can contain newlines.
- A well formed script continues execution of the script until the quit command is hit, even if a malformed command is found in the script.
- A script containing recursion must be let to recurse infinitely, similarly to what the bash shell does. At one point, the "fopen" function returns null and cannot open the file, which ends the recursion.
- A script does not need to be terminated by the ".txt" extension, the filename can be anything.
- Commands are case sensitive.
- Commands can contain more than one space between keywords.
