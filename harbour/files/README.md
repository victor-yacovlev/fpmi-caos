# File properties

## File information

### `stat` structure

Each file in the file system is associated with a meta-information (status), which is defined by the `struct stat` structure:

```
struct stat {
   dev_t     st_dev;         /* ID of device containing file */
   ino_t     st_ino;         /* Inode number */
   mode_t    st_mode;        /* File type and mode */
   nlink_t   st_nlink;       /* Number of hard links */
   uid_t     st_uid;         /* User ID of owner */
   gid_t     st_gid;         /* Group ID of owner */
   dev_t     st_rdev;        /* Device ID (if special file) */
   off_t     st_size;        /* Total size, in bytes */
   blksize_t st_blksize;     /* Block size for filesystem I/O */
   blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

   struct timespec st_atim;  /* Time of last access */
   struct timespec st_mtim;  /* Time of last modification */
   struct timespec st_ctim;  /* Time of last status change */

   /* Backward compatibility */
   #define st_atime st_atim.tv_sec      
   #define st_mtime st_mtim.tv_sec
   #define st_ctime st_ctim.tv_sec
};

```

You can get meta-information about the file using the command `stat FILENAME` or one of the system calls:
 * `int stat(const char *file_name, struct stat *stat_buffer)` - getting information about a file by its name;
 * `int fstat(int fd, struct stat *stat_buffer)` - the same, but for an open file descriptor;
 * `int lstat(const char *path_name, struct stat *stat_buffer)` - similar to `stat`, but if the file name points to a symbolic link, information about the link itself is returned, not the file it refers to.


### Access modes and file types in POSIX

In POSIX there are a few main types of files:

 * Regular file (`S_IFREG = 0100000`). Takes place on the drive; contains the normal data.
 * Directory (`S_IFDIR = 0040000`). A special type of file that stores a list of file names.
 * Symbolic link (`S_IFLNK = 0120000`). A file that references another file (including in a different directory or even on a different file system), and in terms of I/O functions, is no different from the file it references.
 * Block (`S_IFBLK = 0060000`) и character (`S_IFCHR = 0020000`) devices. Used as a convenient way to interact with the equipment.
 * Named pipes (`S_IFIFO = 0010000`) and sockets (`S_IFSOCK = 0140000`) for inter-process communication.

The file type is encoded in the same structure field with access mode (`rwxrwxrwx`) - integer `.st_mode`.

To select individual file types, bitwise operations are performed using one of the macros: `S_ISREG(m)` ` 'S_ISDIR(m)`, `S_ISCHR(m)`, `S_ISBLK(m)`, `S_ISFIFO(m)`, `S_ISLNK(m)' and 'S_ISSOCK(m)`, which return `0` as false and an arbitrary nonzero value as true.

To get the access mode, which is encoded in the lower bits`. st_mode`, you can extract them using bitwise operations with the constants `S_IWUSR`, `S_IRGRP`, `S_IXOTH`, etc. A complete list of constants can be found in `man 7 inode`.

### File access

Each file, in addition to the access mode (`rwx` for owner, group and others) has two identifiers – positive integers:
 * `. st_uid` - ID of the file user-owner;
 * `. st_gid` - ID of the file group-owner.

"Owner" permissions are applied when the current user's ID (obtained by `getuid()`) matches the `.st_uid` field. Similarly, for a group – when `getgid()` matches `.st_gid`. Otherwise, the "other" permissions are applied.

A convenient way to determine the rights of the current user is to use the system call `access`:
```
int access(const char *path_name, int mode)
```

This system call takes as the `mode` parameter a bitwise combination of the flags `R_OK`, `W_OK`, `X_OK` and `F_OK` — respectively, the ability to read, write, execute a file, and its existence. Returns 0 if the listed attributes are valid for the current user, and -1 otherwise.


## File-creation mask

When you create new files using the `open` system call (and all high-level functions that use `open`), you must specify the access mode for the newly created files.

In reality, the access mode may differ from the requested one: for a newly created file (or directory), the *file creation mask* is applied using the bitwise "AND-NOT" operation:
```
 /* Let umask = 0222 */
 open("new_file", O_WRONLY|O_CREAT, 0666); // OK

 /* Created a file with attributes 0666 & ~0222 = 0444 */
```

By default, the file creation mask is `0000` — it does not impose any restrictions. The `umask` system call allows you to explicitly set a new mask that can be used to prevent accidental creation of files with too weak access rights.