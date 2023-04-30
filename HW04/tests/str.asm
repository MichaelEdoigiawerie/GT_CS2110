.orig x0000

AND R1, R1, 0
ADD R1, R1, x0009

AND R2, R2, 0
ADD R2, R2, x0001

AND R7, R7, 0
ADD R7, R7, xffff ; temporarily set the CC to b100

STR R2, R1, 0
STR R2, R1, 1

HALT

.fill x0000
.fill x0000

.end

;; === post-conditions ===
;; R1: x0009, R2: x0001, R7 = xffff, MEM[x0009] = x0001, MEM[x000a] = x0001, CC = b100
