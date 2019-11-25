# Memory pages in virtual address space

## Tools

In addition to the `gdb` step-by-step debugger, there are additional tools for detecting problems when working with memory.

### Interpreted execution with `valgrind`

The `valgrind` toolset uses controlled execution of program instructions, modifying its code before executing on the physical processor.

Main instruments:
 * `memcheck` - diagnostics of memory problems: incorrect heap pointers, duplicated memory freeing, reading uninitialized data and forgotten memory freeing.
 * `callgrind` - diagnostics of running program performance.

To run a program with valgrind, you should build a program with debug information (compile option `-g`), otherwise the output of valgrind will not be informative.

Launching:
```
> valgrind --tool=INSTRUMENT program.jpg ARG1 ARG2 ... ARGn
```

If you use the `callgrind` tool, a `callgrind.out` file is generated after the program is executed. This file has XML format, which can be visualized using KCacheGrind (in KDE for all modern Linux distributions), or its cross-platform equivalent [QCacheGrind](https://sourceforge.net/projects/qcachegrindwin/).

### Runtime error checking with sanitizers

It requires new versions of `clang` or `gcc` and allows you to perform instrumental control during program execution much faster than `valgrind`.

It is implemented using code generation and some functions replacement, for example replacing `malloc`/`free` with the implementation with additional checks.

The main sanitizers:
 * AddressSanitizer (`-fsanitize=address`) - diagnoses situations of memory leaks, memory double freeing, stack or heap overflow, and stack pointers being used after the function terminates.
 * MemorySanitizer (`-fsanitize=memory`) - diagnostics of uninitialized data reading situations. Requires the program and all libraries the program is using to be compiled into position-independent code.
 * Undefined Behavior Sanitizer (`-fsanitize=undefined`) - diagnostics of undefined behavior in integer arithmetic: bit shifts, signed overflow, etc.

## mmap system call

```
#include <sys/mman.h>

void *mmap(
    void *addr,    /* recommended mapping address */
    size_t length, /* length of the mapping */
    int prot,      /* desired memory protection flags */
    int flags,     /* flags for shared mapping */
    int fd,        /* file descriptor */
    off_t offset   /* offset relative to the beginning of the file */
  );

int munmap(void *addr, size_t length) /* unmap existing mapping */
```

The `mmap` system call is intended to create an accessible area with a specific address in the virtual address space of a process. This area can be either associated with a specific file (previously opened) or located in RAM. The second case of usage is usually implemented in the `malloc`/`calloc` functions.

Memory can only be allocated page by page. For most architectures, the size of a single page is 4Kb, although x86_64 processors support larger pages: 2Mb and 1Gb.

In general, you should never rely on a page size of 4096 bytes. It can be found using the `getconf` command or the `sysconf` function:
```
# Bash
> getconf PAGE_SIZE
4096

/* C */
#include <unistd.h>
long page_size = sysconf(_SC_PAGE_SIZE);
```

The `offset` parameter (if a file is used) must be a multiple of the page size; the `length` parameter is not, but the kernel rounds this value to the larger page size. The `addr` parameter (recommended address) can be `NULL`, – in that case the kernel itself assigns an address in the virtual address space.

When using file mapping, the `length` parameter is set to the length of the mapped data; if the file size is smaller than the page size, or the last small part of the file is being mapped, the rest of the page is filled with zeros.

A memory page can have access attributes:
 * reading `PROT_READ`;
 * writing `PROT_WRITE`;
 * execution `PROT_EXE`;
 * nothing `PROT_NONE`.

In the case of mapping to a file, it must be opened for reading or writing according to the required access attributes.

`mmap`flags:
 * `MAP_FIXED` - requires memory to be allocated to the address specified in the first argument; without this flag, the kernel can select the address closest to the address specified.
 * `MAP_ANONYMOUS` - to allocate pages in RAM, not to link to file.
 * `MAP_SHARED` - select pages shared with other processes; in case of file mapping - synchronize changes so that they are available to other processes.
 * `MAP_PRIVATE` - as opposed to`MAP_SHARED`, do not make mapping available to other processes. In the case of mapping to a file, it is readable, and the changes created by the process are not saved to the file.
