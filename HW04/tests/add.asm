.orig x0000

ADD R0, R0, 5
ADD R1, R1, -4

ADD R2, R0, R1

ADD R3, R0, -7

HALT

.end

;; === post-conditions ===
;; R0 = 5, R1 = -4, R2 = 1, R3 = -2, CC = b100
