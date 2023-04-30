.orig x0000

AND R0, R0, 0
ADD R0, R0, x0007

AND R7, R7, 0
ADD R7, R7, xffff ; temporarily set the CC to b100

JMP R0 ; jump to HALT

AND R0, R0, 0 ; proper execution jumps over these two lines
AND R7, R7, 0

HALT
.end

;; === post-conditions ===
;; R0 = x0007, R7 = xffff, PC = x0008, CC = b100
;; If R0 = R7 = 0 then JMP failed!