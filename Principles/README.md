# Principles of Operation

The *principles of operation* for a computer inlcude both its instruction set architecture (ISA) and additional conventions necessary to write working assembly language programs.
These conventions include the memory map, role of supervisor code, application binary interface, and other aspects that control how a program executes in the computer.

## Memory map

As defined by the ISA, the physical memory of the CDC 8600 consists of a vector $`\mathbf{M}[0:N)`$ of 64-bit words.
Although the ISA supports $`N \leq 1048576`$, we are going to consider the specific case of $`N=262144`$, as defined in the reference manual.
By convention, we are going to divide this 256-KiW memory into $`32 \times 8`$-KiW pages. We will assign a specific role to each page.
The CDC 8600 ISA does not know anything about pages.
This is just our convention.

| Page #     | Addresses           | Purpose                  |
|------------|---------------------|--------------------------|
| $`0`$      | $`[     0,  8192)`$ | Context frames           |
| $`1`$      | $`[  8192, 16384)`$ | Supervisor code and data |
| $`2`$      | $`[ 16384, 24576)`$ | User code                |
| $`3`$      | $`[ 24576, 32768)`$ | User static data         |
| $`[4,28)`$ | $`[ 32768,229376)`$ | Heap                     |
| $`28`$     | $`[229376,237568)`$ | Stack for processor 0    |
| $`29`$     | $`[237568,245760)`$ | Stack for processor 1    |
| $`30`$     | $`[245760,253952)`$ | Stack for processor 2    |
| $`31`$     | $`[253952,262144)`$ | Stack for processor 3    |

### The context frames

Page 0 of memory (addresses $`[0,8192)`$) contains $`256 \times 32`$-word context frames.
The first 17 words of a context frame contain an *exchange package*, definig the values of the 16 general-purpose registers (X(0) through X(15)) and an additional *exchange parameter word* (XPW) with various additional registers.

By convention, we are going to reserve all 8 KiWs in page 0 for context frames.
(The ISA does not say you have to reserve all of them.)
For the time being, we are going to assign specific use to the first 8 frames.

| Frame # | Addresses     | Purpose                            |
|---------|---------------|------------------------------------|
| $`0`$   | $`[  0, 32)`$ | Supervisor context for processor 0 |
| $`1`$   | $`[ 32, 64)`$ | Supervisor context for processor 1 |
| $`2`$   | $`[ 64, 96)`$ | Supervisor context for processor 2 |
| $`3`$   | $`[ 96,128)`$ | Supervisor context for processor 3 |
| $`4`$   | $`[128,160)`$ | User context for processor 0       |
| $`5`$   | $`[160,192)`$ | User context for processor 1       |
| $`6`$   | $`[192,224)`$ | User context for processor 2       |
| $`7`$   | $`[224,256)`$ | User context for processor 3       |

## Application binary interface

An *application binary interface* is a set of conventions that supports multiple machine language routines to interoperate with each other.
At this point, we are particularly interested in conventions for *argument passing* between routines.
We will start with a *register-based* argument passing.
Later, as our CDC 8600 assembly code becomes more complex, we may start passing arguments in the stack.
Single-word arguments (integer, double real, addresses) are passed in one X register.
Double-word arguments (double complex) are passed in a pair of X registers.
We pass the first argument in register X0 (if it is a single-word) or register pair <X0,X1> (if it is a double-word) and go from there.
Return values from functions are passed in X0 (single-word) or pair <X0,X1> (double-word).

## Floating-point numbers

The CDC 8600 ISA defines its own floating-point format.
It was common in the 1960's and 1970's, before the standardization of floating-point formats, for each computer vendor to have its own format.
Today, almost everyone uses IEEE 754 floating-point standard, and that is what we are doing.
Four important features of IEEE 754 floating-point:
1. Only a vanishingly small subset of real numbers can be represented as floating-point (even double precision).
2. IEEE 754 can also represent things that are not real numbers: $`+\infty`$, $`-\infty`$, **NaN**s.
3. The bit pattern for represented values is non-obvious.
4. Floating-point arithmetic does not follow the same rules as real number arithmetic.
In particular, floating-point addition and multiplication are not associative.
That is, the order of computation may change the final result.
As a consequence, when comparing floating-point numbers for equality, it is common to compare within a small limit $`\epsilon`$, like $`10^{-9}`$ or $`10^{-12}`$.
Let $`x`$ and $`y`$ be two floating-point numbers.
If $`\mid x - y \mid \lt (\min(\mid y \mid, \mid x \mid) + \epsilon) \times \epsilon`$ then we can treat $`x`$ and $`y`$
as being *close enough*.
