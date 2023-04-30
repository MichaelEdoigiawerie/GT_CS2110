.orig x0000

AND R7, R7, 0
ADD R7, R7, xffff ; temporarily set the CC to b100 

LD R1, MYLABEL ; set the CC to b001

HALT

MYLABEL .fill x0005

.end

;; === post-conditions ===
;; R1 = x0005, R7 = xffff, CC = b001
