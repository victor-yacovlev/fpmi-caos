# Integer arithmetic

## Integer data types

The minimum addressable data size is "typically" one byte (8 bits). "Typically" -- it means that it is not always, and there are different exotic architectures, where "byte" is 9 bits (PDP-10), or specialized signal processors with a minimum addressable data size of 16 bits (TMS32F28xx).

The C standard defines the constant `CHAR_BIT` (in the header file `<limits.h>`), for which it is guaranteed that `CHAR_BIT >= 8`.

A data type representing one byte is historically called a "character" -- `char`, which contains exactly `CHAR_BITS` bits.

The sign of the type `char` is not defined by the standard. For example, it is a signed data type for the x86 architecture, but unsigned -- for ARM. The gcc compiler options `-fsigned-char` and `-funsigned-char` define this behavior.

For other integer data types: `short`, `int`, `long`, `long long`, the C language standard defines the minimum bit size:

| Data type  | Size                              |
| -----------| ----------------------------------|
| `short`    | at least 16 bits                  |
| `int`      | at least 16 bits, usually 32 bits |
| `long`     | at least 32 bits                  |
| `long long`| at least 64 bits, usually 64 bits |

Therefore, you cannot rely on the number of bits in primitive data types, and you should check it with the help of `sizeof` operator, which returns the `number of bytes`, that is, in most cases, how many blocks of size `CHAR_BIT` fit in the data type.

The `long` data type should be treated with extreme caution: on a 64-bit Unix system it is 64-bit, and, for example, on 64-bit Windows it is 32-bit. Therefore, to avoid confusion, this type of data is not allowed.

## Signed and unsigned data types

Integer data types can be preceded by modifiers `unsigned` or `signed`, which indicate the possibility of negative numbers.

For signed types, the high-order bit defines the sign of a number: the value `1` is for negative sign.

The method of internal representation of negative numbers is not regulated by the [C standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570), but all modern computers use the reverse additional code. Moreover, paragraph 6.3.1.3.2 of the C language standard defines a method for converting types from signed to unsigned in such a way that leads to coding with an additional reverse code.

Thus, the value `-1` is represented as an integer, all bits of which are equal to one.

From the point of view of low-level programming, and the C language in particular, the sign of data types determines only the way of applying various operations.

## Data types with a fixed number of bits

Data types that are guaranteed to have a fixed number of digits:
`int8_t`, `int16_t`, `int32_t`, `int64_t` — for signed, and
`uint8_t`, `uint16_t`, `uint32_t`, `uint64_t` — for unsigned,
are defined in header files: `<stdint.h>` (for C99+) and `<cstdint>` (for C++11 and later).

# Overflow

An integer overflow situation occurs when the result data type does not have enough digits to store the final result. For example, if you add the unsigned 8-bit integers: 255 and 1, you get a result that cannot be represented as an 8-bit value.

For **unsigned** numbers the overflow situation is normal, and is equivalent to the operation "addition modulo".

For **signed** data types -- it leads to a situation of *Undefined behaviour*. Such situations cannot occur in correct programs.

Example:
```
int some_func(int x) {
    return x+1 > x;
}
```

It makes sense that such a program should always return a value of `1` (or `true`), since we know that `x+1` is always greater than `x`. The compiler can use this fact to optimize the code, and always return a true value. Thus, the behavior of the program depends on the optimization options that were used.

## Undefined behaviour control

The latest versions of the compilers `clang` and `gcc` (since 6th version) are able to control situations of undefined behavior.

You can enable the generation of *managed* program code that uses additional run-time checks. Certainly, it comes at the cost of some performance degradation.

Such tools are called *sanitizers*, designed for different purposes.

The `-fsanitize=undefined` option is used to enable a sanitizer to monitor the undefined behaviour.

## Overflow control, regardless of sign

Integer overflow means the shift of high-order bit, and many processors, including the x86 family, can diagnose this. C and C++ standards do not provide this capability, but the gcc compiler (since the 5th version) provides **non-standard** built-in functions for performing operations with overflow control.

```
// Addition operation
bool __builtin_sadd_overflow (int a, int b, int *res);
bool __builtin_saddll_overflow (long long int a, long long int b, long long int *res);
bool __builtin_uadd_overflow (unsigned int a, unsigned int b, unsigned int *res);
bool __builtin_uaddl_overflow (unsigned long int a, unsigned long int b, unsigned long int *res);
bool __builtin_uaddll_overflow (unsigned long long int a, unsigned long long int b, unsigned long long int *res);

// Subtraction operation
bool __builtin_ssub_overflow (int a, int b, int *res)
bool __builtin_ssubl_overflow (long int a, long int b, long int *res)
bool __builtin_ssubll_overflow (long long int a, long long int b, long long int *res)
bool __builtin_usub_overflow (unsigned int a, unsigned int b, unsigned int *res)
bool __builtin_usubl_overflow (unsigned long int a, unsigned long int b, unsigned long int *res)
bool __builtin_usubll_overflow (unsigned long long int a, unsigned long long int b, unsigned long long int *res)

// Multiplication operation
bool __builtin_smul_overflow (int a, int b, int *res)
bool __builtin_smull_overflow (long int a, long int b, long int *res)
bool __builtin_smulll_overflow (long long int a, long long int b, long long int *res)
bool __builtin_umul_overflow (unsigned int a, unsigned int b, unsigned int *res)
bool __builtin_umull_overflow (unsigned long int a, unsigned long int b, unsigned long int *res)
bool __builtin_umulll_overflow (unsigned long long int a, unsigned long long int b, unsigned long long int *res)

```
