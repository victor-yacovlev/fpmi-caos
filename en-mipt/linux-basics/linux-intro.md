
# Introduction to Linux OS 

## It is not Windows. Forget everything you know.

### Terminology

 * File system is a hierarchy of files and *directories*. Do not call directories – "folders".
 * Unlike Windows, all files in UNIX are equal, regardless of their name. The concept of "file extension" does not exist, but there are *name suffixes*, separated from the main name by a period, for readability. The filename may have several suffixes, for example `.tar.gz`.
 * The system runs a huge number of *processes*, not "tasks". Processes can be started either directly by the user or by one of the *daemons* that are launched at system startup. Daemons are processes in themselves.


### Common notations of keyboard shortcuts

The following notations of keyboard shortcuts are commonly used for working with UNIX console programs:
 * `C-Letter` - simultaneous pressing `Ctrl` and the letter key. Attention to MacOS users:  `Ctrl` - is exactly the `Ctrl` key, not the `Command`.
 * `M-Letter` - simultaneous pressing `Alt` and the letter key. "M" stands for
 "Meta". There was such a key button on old workstations Sun и SGI.
 * `C-Letter1 Letter2` - simultaneously press `Ctrl` and `Letter1`, then release
 `Ctrl` и press `Letter2`. The same is for `Alt`. Shortcut `C-Letter1` is called *prefix* of a keyboard shortcut. Usually keyboard shortcuts are grouped under the same prefixes for actions of the same nature.
 * `C-Letter1 C-Letter2`. Press `Ctrl`, after that press and release `Letter1`, then  press and release `Letter2`. After that you can release `Ctrl` key.
 * Keys `F13`...`F15`. They are missing on the PC-keyboard.  Pressing can be done with `Shift` and one of the functional keys with number `F...` 10 or 12 less, depending on the terminal. For example, in many graphical terminals `Shift+F5` is for pressing `F15`.


## Start working

Linux, like any other operating system of the UNIX family, is **multiuser** operating system. To get started it is necessary to know your username and password.

There are different ways to login to the system depending on the purposes of using the system.


### Local login with GUI (Graphical User Interface)
This option is typically used for installing Linux as a Desktop.
Usually most Linux distributions provide automatic login if only one user-human was specified during installation (there is also another kind of users –system-users). In case of multiple users, login to the system is pretty similar to the one in Windows or Mac.

After login — a graphical shell (GNOME, Unity or KDE) is displayed.
The command line, that we will mainly work with, is available in Terminal app.

### Local login without GUI
This option is typically used for initial server setup (the graphics stack is a potential "security hole" and usually is not installed) and also for working with embedded systems.

After the system is loaded or the terminal is connected, text message will ask for a user name and a password. And after login to system the control is transferred to the **command interpreter**.

### Remote SSH login
To connect via SSH, you need to use the following command (for Linux/Mac)

```
ssh USERNAME@HOSTNAME
```

There are special programs for SSH connection in Windows, for example [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html).

After the connection you need to enter your password for login. In some cases
there is no need to enter a password, for example, if authorization is configured with using SSH keys.

After login to system the control is transferred to the **command interpreter**.


## Command line working basics
### File system navigation

The command-line prompt usually has an appearance dependent on status:

```
USERNAME@HOSTNAME:WORKING_DIRECTORY>
```

Root directory in file system hierarchy -- is `/`. After system login the working directory is a *home directory* of current user -- this is a directory that is both readable and writable.

The home directories of regular users are located:
 * `/home/` -- for Linux
 * `/usr/local/home` -- for FreeBSD
 * `/Users` -- for MacOS

Regardless of the operating system, the name `~` ("tilde" symbol) is synonymous with home directory of the *current user*.

Tokens `.` and `..`  mean respectively the current directory and the directory
one level higher in the hierarchy.

To navigate through the directories, use the command `cd`. Examples:

```
cd ..          # Navigate one level up
cd ../..       # Navigate two levels up
cd ../src      # Navigate one level up, and then to subdirectory src
cd /           # Navigate to the root directory
cd /usr/lib64  # Navigate to directory /usr/lib64
cd ~/projects  # Navigate to directory /home/NAME/projects
```

**Note**. When entering file or directory names, press `TAB` for autocomplete function.

### Run executable files

Executable file is any file (including a text file) that has special attribute.

There are two ways to run an executable file:
 * Enter the name of this file, if the file is located in one
 of the directories listed in the *environmental variable* `PATH`. All standard UNIX system programs are run in this way.
 * Enter the *full name* of the file. The full name can be either absolute (starts with `/`) or relative (starts with `.`). Programs in the home directory are usually run in this way.


### Default file management programs

 * `cp` -- copy file or directory (with option `-R`)
 * `mv` -- rename (move) file or directory (with option `-R`)
 * `rm` -- remove file or directory (with option `-r`)
 * `ls` -- list working directory contents

All these commands are normal programs that are located in the directory `/usr/bin`.

**Question**. Why isn't there a program called `cd`?
<!--
Correct answer: current directory is the state of interpreter. Every command is run in a separate process that cannot change the state of the parent process.
-->


### Executable file formats

 * Binary file starts with a byte sequence `0x7F 0x45 0x4C 0x46`. This format is called ELF (Executable and Linkable Format).
 * An arbitrary file (including a text file) that starts with a string `#!INTERPRETER_NAME\n`. In this case system starts the specified interpreter and passes the executable file to it as an argument.

 Executable file example:

 ```
 #!/usr/bin/python

 print("Hello, UNIX!")
 ```

### Midnight Commander

Using the command line is not always convenient to navigate the file system.

**Note**. When using the provided VM image, the file system is also
available via FTP: [ftp://student@192.168.56.105/](ftp://student@192.168.56.105/).

*Midnight Commander* -- two-panel file manager available for almost every UNIX-like operating system (including MacOS). It is run with the command `mc`. Working with it is similar to working with FAR Manager or Total Commander. Some keyboard shortcuts are an exception.

Main operations:
 * `F3` -- file view
 * `F4` -- file edit
 * `Shift+F4` -- create and edit new file
 * `F5` -- copy
 * `F6` -- move
 * `F7` -- create directory
 * `F8` -- remove
 * `F10` -- exit Midnight Commander
 * `C-x c` -- edit file permissions
 * `C-x o` -- edit file's user
 * `C-x s` -- create symbolic link for file

**Note.** To exit view or edit mode press `Esc` **twice**. It is so because `Esc` key in classic terminals is intended for prefix input of control characters.


### File system hierarchy

Unlike Windows, where each physical disk or partition on the disk corresponds to a specific letter, for example, `C:\`, the file system tree on UNIX systems shares the root `/`. Separate disks or partitions are *mounted* into subdirectories of the main file system.

The file system of all Linux distributions has the following hierarchy:
 * `/bin` -- executable programs that provide the essential commands
 * `/boot` -- files required to boot the operating system
 * `/dev` -- pseudo-device files
 * `/etc` -- system configuration text files
 * `/home` -- user home directories
 * `/lib` or `/lib64`, or both of them -- the minimum set of shared libraries required for system availability. The `/lib64` directory is present on 64-bit systems and contains variants of libraries for
x86_64, while the `/lib` contains their analogues for i386.
 * `/lost+found` -- files that are out of any directory for some reason (such as an incorrect shutdown of the computer, or disk failure), but their content is available.
 * `/media` -- directory for mounting replaceable media available to all users
 * `/mnt` -- directory for mounting network file systems or foreign sections
 * `/opt` -- directory for installing third-party applications not from the distribution repository, such as Google Chrome
 * `/proc` -- a virtual file system with information about the processes running in the system is mounted here
 * `/root` -- the home directory for the `root` user
 * `/run` -- contains *sockets* and text files with *process IDs* for running daemons
 * `/sbin` -- executable files to be run by `root` user; other users do not have this directory enabled in the environment variable `PATH`, and to run them, you need to specify the full path
 * `/srv` -- files with data for services provided by the system
 * `/sys` -- virtual file system for viewing and modifying kernel settings
 * `/tmp` -- Directory for temporary files
 * `/usr` -- contains a hierarchy similar to the root hierarchy; it contains the files of most of the programs that are installed from distribution *repositories*
 * `/usr/local` -- similar to `/usr`, but is for installing programs yourself from source
 * `/var` -- contains data from various daemons, such as database.

## Console text editors

### Midnight Commander internal editor

Called by pressing `F4` from the file manager or by command `mcedit FILENAME` as an independent program.

Main keys:
 * `F2` -- save file
 * `Esc Esc` -- exit
 * `F3` -- start/end of text selection
 * `F5` -- copy selected text to current position
 * `F6` -- move selected text to current position
 * `F8` -- delete selected text; if no text is selected, -- delete the current line

### VI editor

Because Midnight Commander, and accordingly, the `mcedit` editor are not always installed by default, sometimes there is a need to use the editor `vi`, taht is included in the basic set of almost all Linux distributions.

The editor is started with the command `vi FILENAME` or as a result of some action that requires text editting (for example, `git commit` command -- to edit a commit message).

The editor `vi` can be identified by the black screen, and symbol `~` in all empty lines at the end of the text in the left column of the terminal.

After launching the editor is in **command mode**. No need to press alphanumeric keys to enter text in this mode. If this happens, press `C-[` to return to command mode.

In command mode the text is navigated by the arrow keys, as well as the `h`, `j`, `k` and `l` keys.
In addition to the `vi` text editor, many development environments, such as QtCreator и IntelliJ IDEA, as well as Chrome and Firefox browsers, have plugins that allow you to use VIM-style navigation. So it is better to remember these keys.

To switch to **insert mode**, similar to GUI-editors, you need to press the `i` key. To exit this mode, use the shortcut `C-[`. To switch to **replacement mode** -- there is the `o` key, output is similar.

The basic commands that you need to remember:
 * `:w` -- save file
 * `:e FILENAME` -- open or create a file with the specified name
 * `:q` -- exiting the editor is possible only if there are no unsaved changes
 * `:q!` -- force exit from editor without saving
 * `!COMMAND` -- run a UNIX command without exiting the editor

You can get a more detailed guide to `vi` by running `vimtutor`

### Nano editor

Some distributions, such as Ubuntu, have the `nano` editor installed by default instead of `vi`.
It is an easy to use text editor. It can be identified by the text `GNU nano` in the header at the top of the terminal, and hints about keyboard shortcuts like `^G Get Help` in the basement. The `^` symbol stands for  `Ctrl` key.
