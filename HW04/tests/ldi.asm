.orig x0000

AND R7, R7, 0
ADD R7, R7, xffff ; temporarily set the CC to b100

LDI R1, MYLABEL

HALT

MYLABEL .fill x0005
.fill x0007

.end

;; === post-conditions ===
;; R1 = x0007, R7 = xffff, CC = b001
