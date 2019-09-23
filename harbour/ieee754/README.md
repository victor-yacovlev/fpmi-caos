# Real numbers representation

There are two ways to represent real numbers: with a fixed number of digits for the fractional part (fixed-point), and with a variable number of digits (floating-point).

Fixed-point representation is often used where guaranteed accuracy to a certain digit is required, such as in Finance.

Floating-point representation is more common, and all modern processor architectures operate with this format.


## Floating point numbers in IEE754 format

There are two main types of floating-point objects that are defined by the C standard: `float` (uses 4 bytes for storage) and `double` (uses 8 bytes).

The most significant bit (MSB, also called the high-order bit)
The high-order bit in the number representation indicates the sign of a number. Next, in order of bits, the value of *biased exponent* is stored (8 bits for `float` or 11 bits for `double`), followed by the *mantissa* value (23 or 52 bits).

The biased exponent is necessary in order to be able to store values with a negative exponent in such a representation. The offset for type `float` is `127`, for type `double` -- `1023`.

So, the result value can be calculated like

```
Value = (-1)^S * 2^(E-B) * ( 1 + M / (2^M_bits - 1) )
```

where `S` is the sign bit, `E` is the biased exponent, `B` is the bias offset (127 or 1023), and `M` is the mantissa value, `M_bits` is the number of bits in the exponent.


## How to get the individual bits of a real number

Bitwise operations refer to integer arithmetic, and are not provided for types `float` и `double`. Thus, you need to store a real number in memory, and then read it, interpreting it as an integer. In case of C++, the `reinterpret_cast`operator is used for this. For the C language there are two ways: use pointer casting -- the analog of 'reinterpret_cast', or use the type 'union'.

### Pointers casting
```
// We have some real number that is stored in memory
double a = 3.14159;

// Get a pointer to this number
double* a_ptr_as_double = &a;

// Lose type information by casting it to void*
void* a_ptr_as_void = a_ptr_as_void;

// Void* pointer in C can be assigned to any pointer
uint64_t* a_ptr_as_uint = a_ptr_as_void;

// Well, then just dereferenced pointer
uint64_t b = *a_as_uint;
```

### The use of a type `union`

The `union` type is a data type that is syntactically very similar to the `struct` typeю It means that you can list there several named fields, but conceptually they are completely different data types! If a structure or class has a separate storage space in memoty for each field, this does not happen for `union`, and all fields overlap when placed in memory.

Typically, the `union` type is used as a variant data type (in C++ since the 17th standard `std::variant` is provided for this), but as a side effect -- it is convenient to use type casts in a manner of `reinterpret_cast` ь, without using pointers.

```
// We have some real number that is stored in memory
double a = 3.14159;

// Use union type
typedef union {
    double     real_value;
    uint64_t   uint_value;
} real_or_uint;

real_or_uint u;
u.real_value = a;
uint64_t b = u.uint_value;
```

## Special values in IEEE 754 format

 * Infinity: `E=0xFF...FF`, `M=0`
 * Minus zero (the result of dividing 1 by minus infinity): `S=1`, `E=0`, `M=0`
 * NaN (signaling): `S=0`, `E=0xFF...FF`, `M <> 0`
 * NaN (quiet): `S=0`, `E=0xFF...FF`, `M <> 0`

Some processors, such as the x86 architecture, support an extension of the standard that allows you to more efficiently represent a set of numbers whose values are close to zero. Such numbers are called *denormalized*.

The feature of a denormalized number is the value of the offset exponent `E=0`. In this case, the numerical value is calculated like:

```
Value = (-1)^S * ( M / (2^M_bits - 1) )
```
