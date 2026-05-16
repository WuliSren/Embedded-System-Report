       @ Task 1.2: R0 = R2 - R1
MOV r2, #20       @ Load minuend into R2
MOV r1, #8        @ Load subtrahend into R1
SUB r0, r2, r1    @ R0 = R2 - R1
SWI 0x0           @ Print R0
SWI 0x11          @ Exit