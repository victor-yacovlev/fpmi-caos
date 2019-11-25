# Addressing data in memory and using library functions

* [ARM reference](/practice/asm/arm_basics/arm_reference.pdf)

## Basic commands
As is typical for classical RISC architecture, the ARM processor can only perform operations on registers. Separate commands *load* (`ldr`) and *save* (`str`) are used to access memory.

General form of commands:
```
LDR{condition}{type} Register, Address
STR{condition}{type} Register, Address
```
where `{condition}` - this is a condition of command execution, maybe blank (see previous workshop);`{type}` - data type:
 * `B` - unsigned byte
 * `SB` - signed byte
 * `H` - half-word (16 bit)
 * `SB`- significant half-word
 * `D` - double word.

If type is not specified in the command name, so the common word is implied. Note that to perform data load/save operations that are smaller than the machine word, the signed commands are singled out separately, which make the careful bit extension with zeros, while retaining the senior signed bit.

In case of register loading/saving operations of a pair of registers  (double word), the register must have an even number. The second machine word is implied to be in the neighbouring register numbered `Rn+1`.

## Addressing

The address looks like: `[R_base {, offset}]`, where `R_base` – the name of the register that contains the base address in memory and the optional parameter `offset` – is the offset from the address. The resulting address is defined as `*R_base + offset`.

The offset can be either a register name or a numeric constant encoded into a command. Registers are typically used to index array elements, and constants -- to access structure fields or local variables and arguments relative to `[sp]`.

## Addressing fields of C-structures


According to the C standard, fields in the memory of structures are placed according to the following rules:
 * the order of the fields in memory corresponds to the order of the fields in the structure description
 * the size of the structure must be a multiple of the size of the machine word
 * data inside machine words are placed in such a way as to be pinned to their boundaries.

Thus, the size of the structure does not always match the sum of the sizes of the individual fields. For example:

```
struct A {
  char  f1; // 1 byte
  int   f2; // 4 bytes
  char  f3; // 1 byte
};
// 1 + 4 + 1      = 6 bytes
// size(struct A) = 12 bytes
```

In this example, the field `f1` uses the part of the machine word, the field `f2` - has a size of 4 bytes, so it takes the next machine word, and for the field `f3`  you have to use another one. A simple rearrangement of the fields saves 4 bytes:

```
struct A {
  char  f1; // 1 byte
  char  f3; // 1 byte
  int   f2; // 4 bytes  
};
// 1 + 1 + 4      = 6 bytes
// size(struct A) = 8 bytes
```
In this case, the `f1` and `f3` fields occupy the same machine word.

The GCC compiler has a non-standard `packed` attribute that allows you to create "Packed" structures whose size is equal to the sum of the sizes of its individual fields:

```
struct A {
  char  f1; // 1 byte
  int   f2; // 4 bytes
  char  f3; // 1 byte
} __attribute__((packed));
// 1 + 4 + 1      = 6 bytes
// size(struct A) = 6 bytes
```

## Standard C library functions

Each function that can be used externally has a text label associated with it in the symbol table. After compilation, an entry in the symbol table determines the memory location where the first function statement is placed.

Functions implemented in different object modules but compiled into a single executable file are called in the usual way. The way they are called is no different from calling functions from the same object module.

When using *libraries*, they are loaded into a separate memory area, and at the build stage, the location address of the libraries is not known.

Moreover, the location of the program itself, in general, is also assumed to be unknown.

Such functions that are located in dynamically loaded libraries, including the C standard library, appear in the symbol table with a mark `@plt`. Their implementation in Assembly language looks like:
```
function@plt:

   // Let's load current PC's IP in a temporary register of IP with some offset.
   // The table of real functions, which is filled at the stage of
   // loading the program and dynamic libraries, is located by this offset.
   add  ip, pc, #0
   add  ip, ip, #OFFSET_TO_TABLE_BEGIN

   // Load the address value from this table.
   // This leads to the fact that we jump to the implementation of the real function.
   ldr  pc, [ip, #OFFSET_TO_FUNCTION_INDEX]
   
```

Thus, functions from external libraries are located as if in the program itself, but indeed they represent a "springboard" for performing real functions.
