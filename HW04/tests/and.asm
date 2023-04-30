.orig x0000

AND R0, R0 0
ADD R0, R0, x000b

AND R1, R1, 0
ADD R1, R1, x000d

ADD R7, R7, xffff ; temporarily set the CC to b100

AND R2, R0, R1
AND R3, R0, x000c ; set the CC to b001

HALT

.end

;; === post-conditions ===
;; R0 = x000b, R1 = x000d, R2 = x0009, R3 = x0008, R7 = xffff, CC = b001
