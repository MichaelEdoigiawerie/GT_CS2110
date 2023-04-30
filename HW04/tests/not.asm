.orig x0000

AND R1, R1, 0
ADD R1, R1, x0001

NOT R2, R1

HALT

.end

;; === post-conditions ===
;; R1 = x0001, R2 = xfffe, CC = b100
