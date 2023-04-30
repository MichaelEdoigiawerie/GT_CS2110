.orig x0000

AND R1, R1, 0
ADD R1, R1, x0003
AND R2, R2, 0
ADD R2, R2, x0005

AND R7, R7, 0
ADD R7, R7, xffff ; set the CC to b100

STAR R1, R2

HALT

MYLABEL .blkw 1

.end

;; === post-conditions ===
;; R1 = x0003, R2 = x0005, R7 = xffff, MEM[x0008] = x0007, CC = b100
