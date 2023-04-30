.orig x0000

AND R1, R1, 0
ADD R1, R1, x0009 ; R1 = x0009
AND R2, R2, 0
ADD R2, R2, x0008 ; R2 = x0008 (where the 2C of x0009 will be stored)

AND R7, R7, 0
ADD R7, R7, xFFFF ; set the CC to b001 (positive)

AIOTC R1, R2

HALT

MYLABEL .blkw 1

.end

;; === post-conditions ===
;; R1 = xFFF6, R2 = x0008, R7 = xFFFF, MEM[x0008] = xFFF7 (2C of x0009), 
;; CC = b100 (since the 2C of 9 should be -9, which is negative)
