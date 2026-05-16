@ Task 1.3: R0 = R1 * R2
MOV r1, #6        @ Load multiplicand into R1
MOV r2, #7        @ Load multiplier into R2
MUL r0, r1, r2    @ R0 = R1 * R2
SWI 0x0           @ Print R0
SWI 0x11          @ Exit