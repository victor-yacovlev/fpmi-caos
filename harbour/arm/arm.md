# ARM assembler basics

## Writing and compiling programs

Assembly language programs for the GNU compiler are saved in a file whose name ends in `.s` or `.S`. In the case of `.S` it is assumed that the text of the program can be processed by the preprocessor.

One of the commands is used to compile:
`arm-linux-gnueabi-as` or `arm-linux-gnueabi-gcc`. In the first case, the text is only compiled into an object file, in the second – into an executable program, linked with the standard C library, from which you can use I/O functions.

ARM processors support two sets of commands: the main 32-bit `arm`, and the compacted 16-bit `thumb`. And the processor is able to switch between them. In this workshop, we will use a 32-bit instruction set, so the texts should be compiled with the `-marm` option.

## General syntax

```
// This is a comment (like in C++)

    .text      // the beginning of the section .text with program code
    .global f  // indicates that the f label
               // is externally accessible (similar to extern)
f:             // label (ends with a colon)

     // series of commands
     mul   r0, r0, r3
     mul   r0, r0, r3
     mul   r1, r1, r3
     add   r0, r0, r1
     add   r0, r0, r2  
     mov   r1, r0
     bx    lr
```

## Registers

The processor can only perform operations on *registers* - 32-bit memory cells in the processor core. ARM has 16 registers available programmatically: `r0`, `r1`, ... ,`r15`.

Registers `r13`...`r15` has special assignments and extra names:

 * `r15` = `pc`: Program Counter - pointer to the currently executing instruction
 * `r14` = `lr`: Link Register - stores the return address from the function
 * `r13` = `sp`: Stack Pointer - pointer to the top of the stack.

## Flags

Commands execution may lead to some additional information that is stored in the *flag register*. Flags refer to the last command executed. The main flags are:

 * `C`: Carry - an unsigned overflow occurred
 * `V`: oVerflow - a signed overflow occurred
 * `N`: Negative - negative result
 * `Z`: Zero - zeroing the result.

## Commands

For a complete list of 32-bit commands, see [this reference](/practice/asm/arm_basics/arm_reference.pdf), starting at page 151.

The ARM-32 architecture implies that almost all commands can have *conditional execution*. The condition is encoded with 4 bits in the command itself, and in terms of Assembly syntax, commands can have suffixes.
Thus, each command consists of two parts (without spaces): the command itself and its suffix.

## Basic arithmetic operations

* `AND regd, rega, argb`  // regd ← rega & argb
* `EOR regd, rega, argb`  // regd ← rega ^ argb
* `SUB regd, rega, argb`  // regd ← rega − argb
* `RSB regd, rega, argb`  // regd ← argb - rega
* `ADD regd, rega, argb`  // regd ← rega + argb
* `ADC regd, rega, argb`  // regd ← rega + argb + carry
* `SBC regd, rega, argb`  // regd ← rega − argb − !carry
* `RSC regd, rega, argb`  // regd ← argb − rega − !carry
* `TST rega, argb`        // set flags for rega & argb
* `TEQ rega, argb`        // set flags for rega ^ argb
* `CMP rega, argb`        // set flags for rega − argb
* `CMN rega, argb`        // set flags for rega + argb
* `ORR regd, rega, argb`  // regd ← rega | argb
* `MOV regd, arg`         // regd ← arg
* `BIC regd, rega, argb`  // regd ← rega & ~argb
* `MVN regd, arg`         // regd ← ~argb

## Suffixes-conditions

```
EQ        equal  (Z)
NE        not equal  (!Z)
CS or HS  carry set / unsigned higher or same  (C)
CC or LO  carry clear / unsigned lower  (!C)
MI        minus / negative  (N)
PL        plus / positive or zero  (!N)
VS        overflow set  (V)
VC        overflow clear  (!V)
HI        unsigned higher  (C && !Z)
LS        unsigned lower or same  (!C || Z)
GE        signed greater than or equal  (N == V)
LT        signed less than  (N != V)
GT        signed greater than  (!Z && (N == V))
LE        signed less than or equal  (Z || (N != V))
```

## Transitions

The `pc` counter is automatically incremented by 4 when executed another instruction. Commands are used to branch programs:

 * `B label` - the transition to the label; is used inside of functions for branches associated with loops or conditions
 * `BL label` - save current `pc` to `lr` and switch to `label`; usually used to call functions
 * `BX register` - go to the address specified in the register; usually used to exit functions.
 
## Memory operation

The processor can only perform operations on registers. Special register loading/saving instructions are used to interact with the memory.

* `LDR regd, [regaddr]` – loads the machine word from memory from the address stored in regaddr and stores it in the regd register
* `STR reds, [regaddr]` – stores the machine word in memory at the address, specified in the regaddr register.


# Development for ARM architecture

## Cross-compilation

The process of building programs for a different processor architecture or operating system is called cross-compilation.

This requires a special version of the `gcc` compiler,
designed for a different platform. Many distributions have separate compiler packages for other platforms, including ARM.

In addition, you can download an all-in-one delivery for the ARM architecture from the Linaro project: [http://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/arm-linux-gnueabi/](http://releases.linaro.org/components/toolchain/binaries/7.3-2018.05/arm-linux-gnueabi/).

Full `gcc` command names have the *triplet* form:
```
ARCH-OS[-VENDOR]-gcc
ARCH-OS[-VENDOR]-g++
ARCH-OS[-VENDOR]-gdb

etc.
```
where `ARCH` is the architecture name: `i686`, `x86_64`, `arm`, `ppc`, etc.; `OS` -- the operating system, e.g. `linux`, `win32` or `darwin`; `VENDOR` (optional triplet fragment) -- binary interface agreements (if there are several of them for the platform, for example for ARM this can be a `gnueabi` (standard Linux agreement) or `none-eabi` (no OS, just bare hardware).

The name of the architecture for ARM is often distinguished between `arm` (soft float) and `armhf` (hard float). In the first case, the absence of a floating-point block is implied, so all operations are emulated by software, in the second case they are performed by hardware.


## Running programs for non-native architectures

Execution of programs designed for other architectures is possible only by interpretation of a foreign set of commands. *Emulators* -- special programs intended for this purpose.

ARM architecture, like many other architectures, is supported by the [QEMU emulator](https://www.qemu.org/).

You can emulate either a computer system as a whole, similar to VirtualBox, or only a set of processor commands, using the environment of the Linux host system.

### Running ARM binaries in the native environment

This emulator is included in all common distributions. QEMU commands are like:
```
qemu-ARCH
qemu-system-ARCH
```

where `ARCH` is the name of the architecture to be emulated. Commands, that have `system` in their names, start emulation of a computer system, and you must install an operating system to use them.

Commands without`system` in their names require an executable file name as a mandatory argument in Linux, and emulate only a set of processor commands in *user mode*, executing a "foreign" executable file as if it were a normal program.

Since most programs compiled for ARM Linux use the standard C library, it is necessary to use the ARM version of glibc. A minimal environment with the necessary libraries can be taken from the Linaro project (see link above), and passed to qemu using the `-L PATH_K_SYSROOT` option.

Compile and run example:
```
# assuming the compiler is unpacked in /opt/arm-gcc,
# and sysroot -- in /opt/arm-sysroot

# Compile
> /opt/arm-gcc/bin/arm-linux-gnueabi-gcc -o program hello.c

# The output is an executable file that cannot be executed
> ./program
bash: ./program: cannot execute binary file: Exec format error

# But we can run it with qemu-arm
> qemu-arm -L /opt/arm-sysroot ./program
Hello, World!

```

### Running ARM programs in Raspberry Pi environment emulation

The ideal option for testing and debugging is to use real hardware, such as Raspberry Pi.

If you do not have a computer with an ARM-processor, you can
perform PC emulation with Raspbian system installed.

You can download the image from here: [Google Drive](https://drive.google.com/open?id=11lc_f-_crhP-CJi_FEYb4DE0u9TMViT4)
