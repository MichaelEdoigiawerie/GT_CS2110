.ORIG x0000
    STPCI x002F
    HALT
.END

.ORIG	x0030
    MEM1 .fill x3000
.END

.orig x3000
    MEM2 .fill 10
.end
;MEM[x3000] = PC = 1