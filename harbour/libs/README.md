# Function libraries and their loading

## Functions and pointers to them

Program code in systems with Von Neumann architecture is placed in memory in the same way as regular data.

Thus, it can be loaded or generated while the program is running. Some processors allow you to control which areas of memory can be executed and which are not, and in addition, it is controlled by the kernel. Thus, code can only be executed if it is in memory pages that are marked as executable.


## Typification of function pointers

A Declaration like
```
int (*p_function)(int a, int b);
```
is interpreted as follows: `p_function` is a pointer to a function that takes two integer arguments, and returns a signed integer.

A more general view of a function pointer:
```
typedef ResType (*TypeName)(FuncParameters...);
```

Here `ResType` is the return type of the target function , `TypeName` is the name of type-pointer, `FuncParameters...` - function parameters.

The use of the keyword `typedef` is necessary for the C language not to write the entire type every time (similar to `struct`).

The declaration of pointers to functions is necessary for compiler to know exactly how to use the address of a function, and make possible for compiler to prepare arguments, and understand where to get the return result of the function.


## Libraries

An ELF file can be not only executable file, but also a library that is containing functions. A library differs from an executable in the followinng points:
 * contains a table of available *symbols* - functions and global variables (you can explicitly specify its creation with the `-E` option);
 * can be placed arbitrarily, so the program must be compiled into position-independent code with the option `-fPIC` or `-fPIE`;
 * does not have to have an entry point to the program – functions `_start` and `main`.

The library is compiled using the `-shared` option:
```
 > gcc -fPIC -shared -o libmy_great_library.so lib.c
```

For Linux and xBSD there is a convention for naming libraries: `libNAME.so`, for Mac - `libNAME.dynlib`, for Windows - `NAME.dll`.

Linking a program to a library implies options:
 * `-lNAME` - specifies the library name without the prefix `lib` and the suffix `.so`;
 * `-LPATH` - specifies the directory name to search for used libraries.


## Runtime Search Path

When an ELF file is loaded, all the necessary libraries (on which it explicitly depends) are loaded. You can view the list of dependencies using the `ldd` command.

Libraries are located in one of the default directories: `/lib[64]`, `/usr/lib[64]` or `/usr/local/lib[64]`. Additional directories to search for libraries are defined in the environment variable `LD_LIBRARY_PATH`.

It is possible to explicitly define in the ELF file where to look for the necessary libraries. To do this, use the linker option `ld -rpath PATH`.

To pass options to `ld`, which is called from `gcc`, you can use the `-Wl, OPTION`.

In `rpath` you can specify both absolute paths and the variable `$ORIGIN`, which is equal (when the program is loaded) to the directory containing the program itself. This allows you to create a delivery from program and libraries that are not scattered throughout the file system:

```
 > gcc -o program -L. -lmygreat_library program.c \
       -Wl,-rpath -Wl,'$ORIGIN/'.       
```

This will create an executable file `program` that uses the `libmy_great_library.so`, implying that the library file is in the same directory as the program itself.


## Loading libraries at runtime

Libraries do not have to be tied tightly to the program, and can be downloaded as needed. This uses the `dl` feature set, which was included in the 2001 POSIX standard.

 * `void *dlopen(const char *filename, int flags)` - loads the library file;
 * `void *dlsym(void *handle, const char *symbol)` - searches the library for the required symbol, and returns its address;
 * `int dlclose(void *handle)` - closes the library, and unloads it from memory if it is no longer used in the program;
 * `char *dlerror()` - returns the error text associated with `dl`.

If `dlopen` or `dlsym` are unable to open a file or find a character, a null pointer is returned.

Example of usage is in files: [lib.c](lib.c) and [dynload.c](dynload.c).


## Position-independent executable file

The `-fPIE` option of the compiler indicates that it is necessary to generate position-independent code for `main` and `_start`, and the `-pie` option indicates that you need to specify in the ELF file when linking that it is position-independent.

A position-independent executable file in modern systems is placed at a random address.

If a position-independent executable also contains a table of exported symbols, it is also a library. If there is no `-shared` option, then the compiler assembles the program by removing the character table from it. The `-Wl,-E` option explicitly saves the character table.

Example:
```
  # the abc file.c contains int main() { puts("abc"); }
  > gcc -o program -fPIE -pie -Wl,-E abc.c

  # the program can be run as a regular program
  > ./program
  abc

  # and can be used as a library
  > python3
  >>> from ctypes import cdll, c_int
  >>> lib = cdll.LoadLibrary("./program")
  >>> main = lib["main"]
  >>> main.restype = c_int
  >>> ret = main()
  abc  

 ```
