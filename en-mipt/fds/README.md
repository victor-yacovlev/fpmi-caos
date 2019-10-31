# File input-output

## File descriptors

File descriptors are integers that uniquely identify open files within a single program.

Typically, when a process starts, descriptors `0`, `1`, and `2` are already occupied by the standard input stream (`stdin`), the standard output stream (`stdout`), and the standard error stream (`stderr`).

File descriptors can be created using the `create file` or `open file` operation.


## open/close system calls

The `open` system call is intended to create a file descriptor from an existing file, and has a following signature:

```
int open(const char *path, int oflag, ... /* mode_t mode */);
```

The first parameter is the file name (full, or relative to the current directory). The second parameter — the options for opening the file. The third (optional) is the access rights to the file when it is created.

Main options for opening files:
 * `O_RDONLY` - read only;
 * `O_WRONLY` - write only;
 * `O_RDWR` - read and write;
 * `O_APPEND` - write to end of file (appending);
 * `O_TRUNC` - truncating to length 0;
 * `O_CREAT` - create a file if it does not exist;
 * `O_EXCL` - create a file only if it does not exist.

If successful, a non - negative descriptor number is returned, and if an error occurs, the value `-1` is returned.


## POSIX error handling

The error code of the last operation is stored in the global integer "variable" `errno` (in fact, in modern implementations it is a macro). The values of the codes can be found from the `man` pages, or you can print the error text using the `perror` function.


## POSIX file attributes

When creating a file, the required parameter is a set of POSIX attributes of file access. Basically, they are encoded in octal calculus as `0ugo`, where `u` (user) - access rights for the owner of the file, `g` (group) - access rights for all users of the file group, `o` (others) - for users who are neither the file's owner nor members of the file's group.

In octal notation values from 0 to 7 correspond to a combination of three bits:
```
00: ---
01: --x
02: -w-
03: -wx
04: r--
05: r-x
06: rw-
07: rwx
```


## POSIX read and write

Read and write operations are done using system calls:
```
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```
`buf` is a pointer to the data buffer and `count` is the maximum number of bytes to read / write.

Typically, `count` specifies the size of the data buffer when reading, or the amount of data when writing.

The return type `ssize_t` is an integer defined in the range `[-1...SIZE_MAX]`, where `SSIZE_MAX` is usually the same as `SSIZE_MAX/2`. The value `-1` is used as an error indication; non-negative values are the number of bytes written / read, which may be less than `count`.

If the `read` system call returns `0`, the end of the file has been reached or the input channel has been closed.

## File navigation in POSIX

You can move the current position in the file for ordinary files.
```
off_t lseek(int fd, off_t offset, int whence);
```
This system call is intended to move the current pointer to a file.

The third parameter `whence` is one of the three standard ways to move:
 * `SEEK_SET` - explicitly specify a position in the file;
 * `SEEK_CUR` - move the pointer to a certain offset relative to the current position;
 * `SEEK_END` - move the pointer to a certain offset relative to the end of the file.

The `lseek` system call returns the current position in the file, or `-1` if an error occurs.

The type `off_t` is signed and is 32-bit, by default . In order to be able to work with files larger than 2 gigabytes, the value of the preprocessor variable **is determined before connecting the header files**:
```
#define _FILE_OFFSET_BITS 64
```

In this case, the data type `off_t` becomes 64-bit. You can determine the value of preprocessor variables without changing the source code of the program by passing the `-DKEY=VALUE` option to the compiler:
```
# Compile a program with support for large files
gcc -D_FILE_OFFSET_BITS=64 legacy_source.c
```

## Compiling and running Windows programs from Linux

For cross-compilation, the GCC compiler is used with the target system `w64-mingw`. Can be installed from the package:
 * `mingw32-gcc` - for Fedora
 * `gcc-mingw-w64` - for Ubuntu
 * `mingw32-cross-gcc` - for openSUSE.

You can compile a program for Windows with the command:
```
$ i686-w64-mingw-gcc -m32 program.c
# The output is a.exe file, not a.out
```

Note that the Linux system, unlike Windows, is case-sensitive (distinguishes case of letters in the file system), so you need to use the standard WinAPI header files in lowercase:
```
#include <windows.h> // correct
#include <Windows.h> // compiles in Windows, but not in Linux
```

You can run the resulting file using WINE:
```
$ WINEDEBUG=-all wine a.exe
```

Setting the environment variable `WINEDEBUG` to `- all` causes the console to not display debug information related to the `wine` subsystem, which is mixed with the output of the program itself.

## File descriptors and other data types in WinAPI

For file descriptors in Windows `HANDLE` type is used.

For single-byte strings `LPCSTR` is used, for multi-byte strings `LPCWSTR` is used.

WinAPI functions that have different string support options and work with single-byte functions - end with the letter `A`, and functions that work with multi-byte files - end with the letter `W`.

Type `DWORD` is for an unsigned 32-bit number used for flags.

For a full list of data types, check [Microsoft documentation](https://docs.microsoft.com/en-us/windows/desktop/winprog/windows-data-types).

## WinAPI functions for working with files

The file can be opened using the [CreateFile function](https://docs.microsoft.com/ru-ru/windows/desktop/api/fileapi/nf-fileapi-createfilea).

Read and write — using the [ReadFile](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-readfile) and [WriteFile](https://docs.microsoft.com/en-us/windows/desktop/api/fileapi/nf-fileapi-writefile).

File navigation-using the [SetFilePointerEx function](https://docs.microsoft.com/ru-ru/windows/desktop/api/fileapi/nf-fileapi-setfilepointerex).
