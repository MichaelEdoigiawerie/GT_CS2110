;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - modulus
;;=============================================================
;; Name: Michael Edoigiawerie
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  int x = 17;
;;  int mod = 5;
;;  while (x >= mod) {
;;      x -= mod;
;;  }
;;  mem[ANSWER] = x;

.orig x3000
    ;; YOUR CODE HERE
    LD R0, X; R0 = 17
    LD R1, MOD; R1 = 5

    NOT R2, R1; R2 = ~5
    ADD R2, R2, #1; R2 = -5

WHILE   ADD R3, R0, R2; while (x >= mod)
        BRn ENDWHILE
        ADD R0, R0, R2; x -= mod
        BR WHILE
ENDWHILE
        ST R0, ANSWER; mem[ANSWER] = x
    HALT

    ;; Feel free to change the below values for debugging. We will vary these values when testing your code.
    X      .fill 17
    MOD    .fill 5     
    ANSWER .blkw 1
.end