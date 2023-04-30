.orig x0000
LD R0, NUM1
LD R1, NUM2
LD R3, ZERO
LDAR R2, R0, R1
HALT

NUM1 .fill x3000
NUM2 .fill 1
ZERO .fill 0
.end

.orig x3001
RESULT .fill 5
.end
;R0 = x3000, R1 = 1, R2 = 5, nzp = 0b001