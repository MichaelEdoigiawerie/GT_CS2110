.orig x0000

AND R1, R1, 0
ADD R1, R1, x0007

AND R7, R7, 0
ADD R7, R7, xffff ; temporarily set the CC to b100

LDR R2, R1, 0
LDR R3, R1, 1

HALT

.fill x0006
.fill x0012

.end

;; === post-conditions ===
;; R1 = x0007, R2 = x0006, R3 = x0012, R7 = xffff, CC = b001
