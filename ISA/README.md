# CDC 8600 Instruction Set Architecture

## Architected State

The architected state $` S(N,P) = \langle \mathbf{M}, \mathbf{X} \rangle `$ of a CDC 8600 computer with $` N \leq 1048576`$ words of memory and $` P \leq 4 `$ processors
consists of a vector $` \mathbf{M}[0,N) `$ of 64-bit words (the memory) and a vector $` \mathbf{X}[0,P) `$ of 8-bit context frame indices.
$` \mathbf{M}[n], n \in [0,N) `$ is the $` n `$-th word of memory and $` \mathbf{X}[p], p \in [0,P) `$ is the index of the context frame for processor $` p `$.

The first 8 KiW of memory contain 256 context frames of 32 words each.
Each context frame stores a 17-word *exchange package* with the architected registers of a CDC 8600 processor.
In each context frame, the first 16 words store the contents of registers X0 through X15.
The 17th word stores the contents of the *Exchange Package Word* (XPW), itself consisting of various status and mode registers.
(See page 29 of the reference manual.)

## Arithmetic and Logic Instructions

### Logic Instructions

CDC 8600 logic instructions have the following possible forms:
1. $` X_j \leftarrow X_j \odot X_k `$ (2-operand, also called destructive form)
2. $` X_j \leftarrow X_j \odot n `$ (1-operand and a 6-bit immediate)

| Instruction    | Description                                         | Page in reference manual |
|----------------|-----------------------------------------------------|--------------------------|
| lpjkj(Xj, Xk)  | Logical product of (Xj) and (Xk) to Xj              | 37                       |
| lsjkj(Xj, Xk)  | Logical sum of (Xj) plus (Xk) to Xj                 | 38                       |
| ldjkj(Xj, Xk)  | Logical difference of (Xj) minux (Xk) to Xj         | 39                       |
| copykj(Xj, Xk) | Copy (Xk) to Xj                                     | 40                       |
| rolp(Xj, Xk)   | Rotate (Xj) left by (Xk) into Xj if (Xk) positive   | 42                       |
| shrn(Xj, Xk)   | Shift (Xj) right by (Xk) into Xj if (Xk) negative   | 42                       |
| roln(Xj, Xk)   | Rotate (Xj) left by (Xk) into Xj if (Xk) negative   | 43                       |
| shrp(Xj, Xk)   | Shift (Xj) right by (Xk) into Xj if (Xk) positive   | 43                       |
| popcnt(Xj, Xk) | Population count of (Xk) to Xj                      | 49                       |
| savjn(Xj, n)   | Save lower (Xj) for n bits                          | 117                      |
| blkjn(Xj, n)   | Blank lower (Xj) for n bits                         | 118                      |
| rol((Xj, n)    | Left rotate (Xj) by n bits                          | 119                      |
| shr(Xj, n)     | Right shift (Xj) by n bits                          | 120                      |


### Integer Arithmetic Instructions

CDC 8600 integer arithmetic instructions have the following possible forms:
1. $` X_j \leftarrow X_j \odot X_k `$ (2-operand, also called destructive form)
2. $` X_i \leftarrow X_j \odot X_k `$ (3-operand)
3. $` X_j \leftarrow X_j \odot k `$ (1-operand and a 4-bit immediate)
4. $` X_j \leftarrow X_j \odot K `$ (1-operand and a 20-bit immediate)

| Instruction       | Description                                 | Page in reference manual |
|-------------------|---------------------------------------------|--------------------------|
| ipjkj(Xj, Xk)     | Integer product of (Xj) times (Xk) to Xj    | 52                       |
| xkj(Xj, k)        | Transmit k to Xj                            | 55                       |
| xmkj(Xj, k)       | Transmit -k to Xj                           | 56                       |
| isjkj(Xj, k)      | Integer sum of (Xj) plus k to Xj            | 57                       |
| idjkj(Xj, k)      | Integer difference of (Xj) minus k to Xj    | 58                       |
| idzkj(Xj, Xk)     | Integer difference of zero minus (Xk) to Xj | 62                       |
| xKj(Xj, K)        | Transmit K to Xj                            | 80                       |
| isjKi(Xi, Xj, K)  | Integer sum of (Xj) plus K to Xi            | 121                      |
| isjki(Xi, Xj, Xk) | Integer sum of (Xj) plus (Xk) to Xi         | 122                      |
| idjki(Xi, Xj, Xk) | Integer difference of (Xj) minux (Xk) to Xi | 123                      |

### Floating-point Arithmetic Instructions

CDC 8600 floating-point arithmetic instructions have the following possible forms:
1. $` X_j \leftarrow X_j \odot X_k `$ (2-operand, also called destructive form)
2. $` X_i \leftarrow X_j \odot X_k `$ (3-operand)

| Instruction          | Description                                  | Page in reference manual |
|----------------------|----------------------------------------------|--------------------------|
| fdiv(Xj, Xk)         | Floating divide of (Xj) by (Xk) to Xj        | 48                       |
| fadd(Xi, Xj, Xk)     | Floating sum of (Xj) plus (Xk) to Xi         | 124                      |
| fsub(Xi, Xj, Xk)     | Floating difference of (Xj) minus (Xk) to Xi | 126                      |
| fmul(Xi, Xj, Xk)     | Floating product of (Xj) times (Xk) to Xi    | 128                      |
 
