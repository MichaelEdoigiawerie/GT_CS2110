.orig x0000

LD R0, AA
LD R1, BB
LD R7, ZERO
INC R0, x50
HALT

AA .fill x50
BB .fill 8
ZERO .fill 0

.end
; R0 = x00A0, R1 = 8, nzp = 0b001