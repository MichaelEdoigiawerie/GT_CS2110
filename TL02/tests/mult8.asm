.orig x0000
LD R0, AA
LD R1, BB
LD R2, CC
MULT8 R0
MULT8 R1
MULT8 R2
HALT
.end
AA .fill x0000
BB .fill x0005
CC .fill xFFFF

;R0 = x0000  (0)
;R1 = x0014  (20)
;R2 = xFFFC  (-8)