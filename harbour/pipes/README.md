# Duplicating file descriptors. Pipes.

## Duplicating file descriptors

The `fcntl` system call allows you to configure various manipulations on open file descriptors. One of the manipulation commands is `F_DUPFD` - creating a *copy* of the descriptor in the current process, but with a different number.

A copy implies that two different file descriptors are associated with the same open file in the process, and share the following attributes:
 * the file object itself;
 * locks associated with the file;
 * the current position of the file;
 * open mode (read /write/add).

At the same time, the `CLOEXEC` flag is not saved. I means the automatic closing of the file when the `exec` system call is executed.

POSIX system calls `dup` and `dup2` provide simplified semantics for creating a copy of file descriptors:
```
#include <unistd.h>

/* Returns a copy of the new file descriptor, and, similar to `open`, the numeric value of the new file descriptor is the minimum unoccupied number. */
int dup(int old_fd);

/* Creates a copy of a new file descriptor with an explicit new_fd number. If the file descriptor new_fd was previously opened, it closes it.*/
int dup2(int old_fd, int new_fd);
```

## Unnamed pipes


A pipe is a pair of related file descriptors, one of which is read-only and the other is write-only.

The channel is created using the `pipe` system call:
```
#include <unistd.h>

int pipe(int pipefd[2]);
```

As an argument, the system call `pipe` gets a pointer to an array of two integers, where the file descriptor numbers will be written:
 * `pipefd[0]` - read-only file descriptor;
 * `pipefd[1]` is a file descriptor intended for writing.

### Writing data to pipe

It is done using the system call `write`. It's first argument is `pipefd[1]`. The channel is buffered. In Linux its size is usually 65K. Possible scenarios of writing behavior:

 * the `write` system call terminates immediately if the data size is smaller than the buffer size and there is enough space in the buffer;
 * the `write` system call pauses execution until there will be enough space in the buffer (until the previous data will not be read by someone from the channel);
 * the `write` system call fails with a `Broken pipe` error (delivered via the `SIGPIPE` signal) if the channel on the opposite side has been closed and there is no one to read the data.

### Reading data from a pipe

It is done using the system call `read`. It's first argument is `pipefd[0]`. Possible reading behavior scenarios:

* if there is data in the pipe's buffer, the system call reads it and exits;
 * if the buffer is empty and there is **at least one** open file descriptor on the opposite side, the execution of `read` is blocked;
 * if the buffer is empty and all file descriptors on the opposite side of the pipe are closed system call `read` immediately shuts down, returning `0`.


### dead lock problem

When executing `fork`, `dup`, or `dup2` system calls, copies of the file descriptors, associated with the pipe, are created. If you do not close all unnecessary (unused) copies of file descriptors intended for writing, it leads to the fact that when you try to read from the pipe, `read` will be waiting for data instead of shutting down.

```
int fds_pair[2];
pipe(fds_pair);

if ( 0!=fork() )  // now we have an implicit copy of the file descriptors
{
    // write some data to the buffer
    static const char Hello[] = "Hello!";
    write(fds_pair[1], Hello, sizeof(Hello));
    close(fds_pair[1]);

    // and now read it back
    char buffer[1024];
    read(fds_pair[0], buffer, sizeof(buffer)); // get deadlock!
}
else while (1) shched_yield();
```

To avoid this problem, you should check carefully when copies of file descriptors are created and close them when they are not needed.
