# Assignment 1: mysh

## Introduction
This shell was created by Tristan Bouchard in January of 2020. It currently supports the commands specified when running the "help" command. The source code can be compiled both on Windows and Linux machines using the supplied Makefile. It can also be compiled with debugging symbols using the `make DEBUG=1` flag, which enables the required symbols to debug with gdb.

The resulting executable, `mysh.exe` or `mysh` depending on the machine it was compiled on, can then be run on its own:

> `mysh.exe` or `./mysh`

 or with a well-formed scripts as standard input,

> `mysh.exe < wellFormedScript.txt`

or
>`./mysh < wellFormedScript.txt`

See the [Script assumptions](#scriptAssumptions) section below for more information on well-formed scripts.


## Setting Variables
This shell is capable of storing up to SHELL_MEMORY_SIZE variables, which are case sensitive. Only the first entry after the variable will be taken into account to set the variable. For example, the command:

> `set bob Montreal Canadiens`

will set the variable `bob` to `Montreal`. Trying `print Bob` (with an uppercase 'b') will result in a `Variable does not exist` message, only `print bob` (all in lowercase, as in the command) will print `Montreal`

## Size assumptions
This assignment makes the following assumptions about the input size and restrics the shell memory in the following way:

- Shell memory is limited to 100 variables, but can be modified by changing the SHELL_MEMORY_SIZE variable in "shell.h"
- Input size is limited to 1000 characters, but can be modified by changing the INPUT_SIZE variable in "shell.h"

## Script assumptions<a name="scriptAssumptions"></a>
This shell also assumes certain specifications, based on information gathered on Piazza and through questions with the professor:
- A well formed script must end with a "quit" command.
- A well formed script can contain newlines.
- A well formed script continues execution of the script until the quit command is hit, even if a malformed command is found in the script.
- A script containing recursion must be let to recurse infinitely, similarly to what the bash shell does. At one point, the "fopen" function returns null and cannot open the file, which ends the recursion.
- A script does not need to be terminated by the ".txt" extension, the filename can be anything.
- Commands are case sensitive.
