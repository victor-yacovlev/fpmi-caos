# x86 assembler (32-bit, and a few words about 64-bit)
The main reference for the set of commands [(converted to HTML)](https://www.felixcloutier.com/x86/).

Reference for the MMX, SSE, and AVX command sets [on the Intel website](https://software.intel.com/sites/landingpage/IntrinsicsGuide/).

Good tutorial on x86 Assembly [on WikiBooks](https://en.wikibooks.org/wiki/X86_Assembly)

## 32-bit assembler on 64-bit systems

We will use a 32-bit instruction set. On 64-bit architectures, the GCC compiler option `-m32` is used for this.

It is also necessary to install the 32-bit library stack. On Ubuntu it is done with only one command:
```
sudo apt-get install gcc-multilib
```

## Intel and At&T syntax

Historically, there are two x86 Assembly language syntaxes: AT&T syntax – used in UNIX systems, and Intel syntax – used in DOS/Windows.

The difference is primarily in the order of command arguments.

The gcc compiler uses AT&T syntax by default, but can switch to Intel syntax with `-masm=intel` option.

You can also specify the syntax to use in the first line in the text
of the program itself:
```nasm
.intel_syntax noprefix
```

The parameter `noprefix` after `.intel_syntax` indicates here that in addition to the order of the arguments corresponding to the Intel syntax, register names should not begin with the `%` character, and constants should not begin with the `$` character, as it is customary to do in the AT&T syntax.

We will use this syntax because it is the syntax that most of the available documentation and examples are written with, including documentation from processor manufacturers.

## General purpose processor registers

Historically, the x86 processor family inherited a set of 8-bit General-purpose registers of the 8080/8085 family called `a`, `b`, `c` and `d`. But since the 8086 processor became 16-bit, the registers were named `ax`, `bx`, `cx`and `dx`.
In 32-bit processors they are called `eax`, `ebx`, `ecx` and `edx`, in 64-bit `rax`, `rbx`, `rcx` and `rdx`.

In addition, x86 has "dual-purpose" registers, which can be used, among other things, as General-purpose registers, if you use a limited subset of processor commands:

 * `ebp` - the upper boundary of the stack;
 * `esi` - index of the array element that is a source for copy operation;
 * `edi` - index of the array element that is a destination for copy operation.

The `esp` register contains a pointer to the lower boundary of the stack, so it is not recommended to use it arbitrarily.

### x86-64 registers

64-bit registers for the x86-64 architecture are named starting with the letter `r`. In addition to the registers `rax`...`rsi`,`rdi` general purpose registers`r9`...`r15` can be used. The stack pointer is stored in `rsp`, the upper bound of the stack frame is stored in `rbp`.

The lower 32-bit parts of the `rax`...`rsi`,`rdi`,`rsp`,`rbp`  registers can be addressed by the names `eax`...`esi`,`edi`,`esp`, `ebp`. When writing values to 32-bit register names, the highest 32 digits are zeroed, which is acceptable for operations on 32-bit unsigned values.

To work with signed 32-bit values, such as the `int` type, you must first perform the *sign extension* operations with the `movslq` command.

## Some instructions

**For Intel syntax**, the first argument of the command is the one whose value will be modified, and the second – the one which remains unchanged.

```nasm
add     DST, SRC        /* DST += SRC */
sub     DST, SRC        /* DST -= SRC */
inc     DST             /* ++DST */
dec     DST             /* --DST */
neg     DST             /* DST = -DST */
mov     DST, SRC        /* DST = SRC */
imul    SRC             /* (eax,edx) = eax * SRC - signed */
mul     SRC             /* (eax,edx) = eax * SRC - unsigned */
and     DST, SRC        /* DST &= SRC */
or      DST, SRC        /* DST |= SRC */
xor     DST, SRC        /* DST ^= SRC */
not     DST             /* DST = ~DST */
cmp     DST, SRC        /* DST - SRC, the result is not saved, */
test    DST, SRC        /* DST & SRC, the result is not saved  */
adc     DST, SRC        /* DST += SRC + CF */
sbb     DST, SRC        /* DST -= SRC - CF */
```

**For AT&T syntax** the order of arguments is the opposite. That is, the command
`add %eax, %ebx` will calculate the sum of `%eax` and `%ebx` , then save the result
in register `%ebx`, which is specified as the second argument.

## Processor flags

Unlike ARM processors, where the flag register is updated
only if there is a special flag in the command, denoted by a suffix
`s`, in Intel: processors flags are always updated by most instructions.

The `ZF` flag is set if the result of operation is zero.

The `SF` flag is set if the result of the operation is a
negative number.

The `CF` flag is set if the operation results in a transfer from the highest bit of the result. For example, `CF` is set for addition operation if the result of addition of two unsigned numbers cannot be represented by a 32-bit unsigned number.

The `OF` flag is set if the operation results in an overflow of the signed result. For example, when adding, `OF` is set if the result of adding two signed numbers cannot be represented by a 32-bit signed number.

Note that both addition `add` and subtraction `sub` operations set both the `CF` and the `OF` flags. Addition and subtraction of signed and unsigned numbers are executed exactly in the same way, and so only one instruction is used for both signed and unsigned operations.

The `test` and `cmp` instructions do not save the result but only change the flags.

## Control the execution order of the program

Unconditional jump is performed using the `jmp` statement
```nasm
jmp label
```

Conditional jumps check combinations of arithmetic flags:
```nasm
jz      label   /* jump, if equal to (zero), ZF == 1 */
jnz     label   /* jump, if not equal (not zero), ZF == 0 */
jc      label   /* jump, if CF == 1 */
jnc     label   /* jump, if CF == 0 */
jo      label   /* jump, if OF == 1 */
jno     label   /* jump, if OF == 0 */
jg      label   /* jump, if greater for signed numbers */
jge     label   /* jump, if >= for signed numbers */
jl      label   /* jump, if < for signed numbers */
jle     label   /* jump, if <= for signed numbers */
ja      label   /* jump, if > for unsigned numbers */
jae     label   /* jump, if >= (unsigned) */
jb      label   /* jump, if < (unsigned) */
jbe     label   /* jump, if <= (unsigned) */
```

Function call and return are performed by `call` and `ret` commands
```nasm
call    label   /* pushes the return address to the stack, and jumps to label */
ret             /* pulls the return address from the stack and navigates to it */
```

Besides that there is a complex command for loops organization, which implies that the `ecx` register contains a loop counter:
```nasm
loop    label   /* decreases the ecx value by 1; if ecx==0, then
                   jump to the next instruction, otherwise
                   jump to label */
```

## Memory addressing

Unlike RISC processors, x86 use **one of the command arguments** as an address in memory.

**In AT&T syntax** this addressing is written as: `OFFSET(BASE, INDEX, SCALE)`, where `OFFSET` – is a constant, `BASE` and `INDEX` – are registers, and `SCALE` -  is one of the values: `1`,  `2`,  `4` or `8`.

The memory address is calculated as `OFFSET+BASE+INDEX*SCALE`. `OFFSET`,
`INDEX` and `SCALE` parameters are optional. In their absence it is implied,
that `OFFSET=0`, `INDEX=0`, `SCALE` is equal to the size of the machine word.

**Intel syntax** uses a more obvious notation: `[BASE + INDEX * SCALE + OFFSET]`.


## Calling conventions for 32-bit architecture

The return value of the 32-bit function type is written to the register `eax`. The pair `eax` and `edx` is used to return the 64-bit value.

The called function must store the values of the general-purpose registers `ebx`, `ebp`, `esi` and `edi` on the stack.

Arguments can be passed to a function in different ways, depending on the conventions of the ABI.


### Cdecl and stdcall conventions

Argument passing conventions used on 32-bit x86 systems. All function arguments are stacked right-to-left, then a function is called that addresses the arguments through a `ebp` or `esp` pointer with some positive offset.

Example:
```c
char * s = "Name";
int value1 = 123;
double value2 = 3.14159;
printf("Hello, %s! Val1 = %d, val2 = %g\n", s, value1, value2);
```

Here, before calling `printf`, the values of the variables will be stacked before the function is called:
```nasm
push    value2
push    value1
push    s
push    .FormatString
call    printf
```

If the `stdcall` convention is used, **called** function must remove its arguments from the stack after they were used.

If the `cdecl` convention is used, the **calling** function must remove from the stack those variables that were passed to the called function.

In C/C++, the conventions that are used can be specified in functions specifiers, for example:
```
void __cdecl regular_function(int arg1, int arg2);
#define WINAPI __stdcall
void WINAPI  winapi_function(int arg1, int arg2);
```

The `stdcall` convention is now used primarily in the Windows operating system to refer to WinAPI functions. In all other cases on 32-bit systems  `cdecl` convention is used.

### fastcall convention

If you want to pass some integer arguments to a function, you can use registers, as in the ARM architecture. This agreement is called `fastcall`.


The `fastcall` convention is used to call kernel functions (system calls) on UNIX-like systems. In particular, Linux uses the `eax` register to pass the system call number, and the `ebx`, `ecx`, and `edx` registers – to pass integer arguments.

A similar approach is used in the x86-64 architecture, where there are more registers available than in the 32-bit x86 architecture.

## Calling conventions for 64-bit architecture AMD64 SystemV ABI

Integer arguments are passed sequentially in registers: `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9`. If more than 6 arguments are passed, the remaining arguments are passed through the stack.

Real arguments are passed through registers `xmm0`...`xmm7`.

The return value of the integer type must be stored in `rax`, for real type – in `xmm0`.

The called function must store the values of general-purpose registers `rbx`, `rby`, and registers `r12`...`r15` on the stack.

In addition, when calling a function for a 64-bit architecture, there is an additional requirement – before calling the function, the stack must be aligned to the boundary of 16 bytes, that is, you must reduce the value of `rsp` so it was a multiple of 16. If a stack is used to pass parameters apart from registers, these parameters must be pinned to the bottom aligned edge of the stack.

Functions are guaranteed a 128-byte "red zone" in the stack below the `rsp` register - an area that will not be affected by any external event, such as a signal handler. Thus, it is possible to use memory up to `rsp-128` for addressing local variables.
