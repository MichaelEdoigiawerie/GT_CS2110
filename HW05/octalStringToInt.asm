;;=============================================================
;; CS 2110 - Spring 2023
;; Homework 5 - octalStringToInt
;;=============================================================
;; Name: Michael Edoigiawerie
;;=============================================================

;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String octalString = "2110";
;;  int length = 4;
;;  int value = 0;
;;  int i = 0;
;;  while (i < length) {
;;      int leftShifts = 3;
;;      while (leftShifts > 0) {
;;          value += value;
;;          leftShifts--;
;;      }
;;      int digit = octalString[i] - 48;
;;      value += digit;
;;      i++;
;;  }
;;  mem[mem[RESULTADDR]] = value;

.orig x3000
    ;; YOUR CODE HERE
    LD R0, LENGTH; R0 = length
    LD R1, VALUE; R1 = value
    LD R2, INDEX; R2 = index

    NOT R0, R0; R0 = ~length
    ADD R0, R0, #1; R0 = -length

WHILE   ADD R3, R2, R0; while (i < length)
        BRzp ENDWHILE
        LD R4, LEFTSHIFTS; R4 = leftShifts
        W1  ADD R4, R4, #0; while (leftShifts > 0)
            BRnz    ENDW1
            ADD R1, R1, R1; value += value
            ADD R4, R4, #-1; leftShifts--
            BR  W1
        ENDW1
        LD  R4, OCTALSTRING; R4 = octalString
        ADD R4, R4, R2; R4 = octalString + index
        LDR R4, R4, #0; R4 = octalString[i]
        ADD R4, R4, #-16; R4 = octalString[i] - 16
        ADD R4, R4, #-16; R4 = octalString[i] - 32
        ADD R4, R4, #-16; R4 = octalString[i] - 48
        ADD R1, R1, R4; R1 += R4
        ADD R2, R2, #1; R2++
        BR  WHILE

LEFTSHIFTS  .fill 3
VALUE   .fill 0
INDEX   .fill 0

ENDWHILE
            STI R1, RESULTADDR; mem[mem[RESULTADDR]] = value
            HALT
    
;; Do not change these values! 
;; Notice we wrote some values in hex this time. Maybe those values should be treated as addresses?
ASCII           .fill -48
OCTALSTRING     .fill x5000
LENGTH          .fill 4
RESULTADDR      .fill x4000
.end

.orig x5000                    ;;  Don't change the .orig statement
    .stringz "2110"            ;;  You can change this string for debugging!
.end
