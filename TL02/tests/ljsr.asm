.orig x0000

LD R7, ZERO
LJSR TARGET 
HALT

ZERO .fill 0
.end

.orig x0030

TARGET LD R0, AA
HALT

AA .fill 6
.end

; R0 = 6, nzp = 0b001