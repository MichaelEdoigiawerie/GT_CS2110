.orig x0000

AND R7, R7, 0
ADD R7, R7, xffff ; temporarily set the CC to b100

LEA R2, MYLABEL ; CC should not change

HALT

MYLABEL .fill x0000

.end

;; === post-conditions ===
;; R2 = x0004, R7 = xffff, CC = b100
