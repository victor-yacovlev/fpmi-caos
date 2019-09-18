
# Developer tools

## Compilers: `gcc` and `clang`

The standard delivery of modern UNIX systems includes one of the compilers: either `gcc` or `clang`. By default, `gcc` is used in Linux and `clang` -- in BSD-systems. Working with `gcc` compiler will be described below. But keep in mind that working with `clang` is very similar. Both compilers have a lot in common, including command-line options.

In addition, there is the `cc` command, which is a symbolic link to the default C compiler (`gcc` or `clang`), and the `c++` command, which is a symbolic link to the default C++ compiler.

Let's consider a simple program in C++:
```
// file hello.cpp
#include <iostream>

int
main() {
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
```

You can compile this program by using the command:
```
> c++ -o program.jpg hello.cpp
```

The compiler option `-o FILENAME` specifies the name of the output file to be created. The default name is `a.out`. Pay attention that `program.jpg` is an executable file!

### Stages of compiling a С or C++ program

When you run `c++ -o program.jpg hello.cpp` command, a quite complex chain of actions is performed:

 1. *Preprocessing* of text file `hello.cpp`. At this stage, the *preprocessor directives* are processed (that begin with the `#` character), and after that the new program text is obtained. If you run the compiler with the `-E` option, only this step will be executed, and the converted text of the program will be output to standard output stream (stdout).

 2. *Translating* one or more C/C++ texts into object modules that contain machine code. If you specify the `-c` option, the build of the program will be stopped at this stage and object files with the suffix `o` will be created. Object files contain *binary* executable code that corresponds exactly to some Assembly language text. This text can be obtained using the `-S` option. In this case, text files with the suffix `.s` will be created instead of object files.

 3. *Linking* one or more object files into an executable file and linking it to the C/C++ standard library (or other libraries, if required). The compiler calls the third-party program `ld` to perform the linking.

### C programs v.s. C++ programs

The `gcc` compiler has the `-x LANGUAGE` option to specify the source language of the program: C (`c`), C++ (`c++`) or Fortran (`fortran`). By default, the source language is defined according to the filename: `.c` is a program in C language, and the file with the name ending in `.cc`, `.cpp` or `.cxx` -- is a C++ text. Therefore, the filename is significant.

This applies to the preprocessing and translation stages, but it can cause problems in the build stage, too. For example, when using the `gcc` command instead of `g++` (or `cc` instead of `c++`) you can successfully compile a program's source code in C++, but you will encounter errors during the linking phase because the options passed to the `ld` linker bind only to the C standard library, not to C++ one. Therefore, when building programs in C++, you need to use the command `c++` or `g++`.

### Standard specifying

The compiler option `-std=NAME ' allows you to explicitly specify the language standard to be used. It is recommended to specify explicitly the standard to use because the default behavior depends on the Linux distribution you are using. Valid names:
 * `c89`, `c99`, `c11`, `gnu99`, `gnu11` for C;
 * `c++03`, `c++11`, `c++14`, `c++17`, `gnu++11`, `gnu++14`, `gnu++17` for C++.

A double-digit number in the name of standard indicates its year. If `gnu` is in the standard name, GNU compiler extensions are implied (specific to UNIX-like systems), and the `#define _DEFAULT_SOURCE` macro is also considered to be defined, that in some cases changes the behaviour of individual functions of the standard library.

In the future, we will focus on the standard `c11`, and in some tasks, where it will be explicitly stated -- on its extension `gnu11`.

## Object files, libraries and executable files

### Python interpreter module `ctypes` 

Consider a [program](my-first-program.c) in C:
```
/* my-first-program.c */
#include <stdio.h>

static void
do_something()
{
    printf("Hello, World!\n");
}

extern void
do_something_else(int value)
{
    printf("Number is %d\n", value);
}

int
main()
{
    do_something();
}
```

Let's compile this program into an object file, and then get from it: (1) an executable program; (2) a shared library. pay attention to the `-fPIC` option for generating position-independent code, which will be discussed in a later seminar.

```
> gcc -c -fPIC my-first-program.c
> gcc -o program.jpg my-first-program.o
> gcc -shared -o library.so my-first-program.o
```

As a result, we get the program `program.jpg`, which outputs the line `Hello, World!` and a *library* with the name `library.so` that can be both used from C/C++ programs, and dynamic loaded for using by the Python interpreter:

```
> python3
Python 3.6.5 (default, Mar 31 2018, 19:45:04) [GCC] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> from ctypes import cdll
>>> lib = cdll.LoadLibrary("./library.so")
>>> lib.do_something_else(123)
>>> retval = lib.do_something_else(123)
Number is 123
>>> print(retval)
14
```

Note that the result of  `do_something_else` function is a mysterious number `14` (it may be other when you try to reproduce this experiment), although the function returns `void`.

It is so because shared libraries store only the **names** of the functions, not their signatures (parameter and return value types).

Attempt to call the `do_something` function will fail:
```
>>> lib.do_something()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/usr/lib64/python3.6/ctypes/__init__.py", line 361, in __getattr__
    func = self.__getitem__(name)
  File "/usr/lib64/python3.6/ctypes/__init__.py", line 366, in __getitem__
    func = self._FuncPtr((name_or_ordinal, self))
AttributeError: ./library.so: undefined symbol: do_something
```

In this case, the name `do_something` is not found because in the C source text, the `static` modifier before the function name explicitly prohibits the usage of the function anywhere outside the current source text.

### Display the symbol-table

To examine object files, including linked ones, you can use the `objdump` utility.

The `--syms` or `-t` options display the separate sections of the executable file that are named with *characters*.

Some names are marked as '*UND*', which means that the name is used in the object file, but its location is unknown. The task of the linker is just to find the required names in different object files or dynamic libraries, and then - to substitute the correct address.

Some characters are marked as global (the `g` character in the second column of the output) and some are marked as local (the `l` character). Non-global characters are considered *non-exportable*, that means (theoretically) they should not be accessible from the outside.


## Debugger

If you compile a program with the `-g ` option, the size of the program will increase because it has now additional sections that contain *debugging information*.

Debug information contains information about the correspondence of individual fragments of the program to the source code, and includes line numbers, source file names, type names, functions and variables.

This information is used only by the debugger, and has almost no effect on the behavior of the program. Thus, debugging information can be combined with optimization, even with the quite aggressive one (compiler option `-O3`).

For debugging use the `gdb` command with the name of the executable file or command as an argument.

Basic `gdb` commands:
 * `run` -- run the program, you can pass arguments after `run`;
 * `break` -- set a break-point, the parameters for this command may be either a function name or a couple of `FILENAME:LINENUMBER`;
 * `ni`, `si` -- step over the function or step into respectively;
 * `return` -- step out of the function;
 * `continue` -- continue to the next breakpoint or exception;
 * `print` -- prints the value of a given expression for a current context.

Interaction with the debugger is performed in the command line mode. Various integrated development environments  (CLion, CodeBlocks, QtCreator) are just graphical shells that use this particular debugger and visualise the interaction with it.

A more detailed list of commands can be found in [CheatSheet](https://www.cheatography.com/fristle/cheat-sheets/closed-source-debugging-with-gdb/).
