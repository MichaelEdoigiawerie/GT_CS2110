.orig x0000

AND R1, R1, 0
ADD R1, R1, x0008

AND R7, R7, 0
ADD R7, R7, xffff ; set the CC to b100

STI R1, MYLABEL

HALT

MYLABEL .fill x0007
.fill x0000

.end

;; === post-conditions ===
;; R1 = x0008, R7 = xffff, MEM[x0007] = x0008, CC = b100
