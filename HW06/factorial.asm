;;=============================================================
;;  CS 2110 - Spring 2023
;;  Homework 6 - Factorial
;;=============================================================
;;  Name: Michael Edoigiawerie
;;============================================================

;;  In this file, you must implement the 'MULTIPLY' and 'FACTORIAL' subroutines.

;;  Little reminder from your friendly neighborhood 2110 TA staff: don't run
;;  this directly by pressing 'Run' in complx, since there is nothing put at
;;  address x3000. Instead, call the subroutine by doing the following steps:
;;      * 'Debug' -> 'Simulate Subroutine Call'
;;      * Call the subroutine at the 'MULTIPLY' or 'FACTORIAL' labels
;;      * Add the [a, b] or [n] params separated by a comma (,) 
;;        (e.g. 3, 5 for 'MULTIPLY' or 6 for 'FACTORIAL')
;;      * Proceed to run, step, add breakpoints, etc.
;;      * Remember R6 should point at the return value after a subroutine
;;        returns. So if you run the program and then go to 
;;        'View' -> 'Goto Address' -> 'R6 Value', you should find your result
;;        from the subroutine there (e.g. 3 * 5 = 15 or 6! = 720)

;;  If you would like to setup a replay string (trace an autograder error),
;;  go to 'Test' -> 'Setup Replay String' -> paste the string (everything
;;  between the apostrophes (')) excluding the initial " b' ". If you are 
;;  using the Docker container, you may need to use the clipboard (found
;;  on the left panel) first to transfer your local copied string over.

.orig x3000
    ;; You do not need to write anything here
    HALT

;;  MULTIPLY Pseudocode (see PDF for explanation and examples)   
;;  
;;  MULTIPLY(int a, int b) {
;;      int ret = 0;
;;      while (b > 0) {
;;          ret += a;
;;          b--;
;;      }
;;      return ret;
;;  }

MULTIPLY ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the MULTIPLY subroutine here!

    ;Stack Build Up
    ADD R6, R6, #-4; Push the Return Value, Return Address, Old Frame Pointer, and Local Variables onto the Stack
    STR R7, R6, #2; Save The Return Address to the Stack
    STR R5, R6, #1; Save The Old Frame Pointer to the Stack
    ADD R5, R6, #0; Make the Current Frame Pointer point to the First Local Variable
    ADD R6, R6, #-5; Push The Five General Purpose Registers Onto The Stack
    STR R0, R5, #-1; Save old value of R0 to the Stack
    STR R1, R5, #-2; Save old value of R1 to the Stack
    STR R2, R5, #-3; Save old value of R2 to the Stack
    STR R3, R5, #-4; Save old value of R3 to the Stack
    STR R4, R5, #-5; Save old value of R4 to the Stack
    
    ;Work of the Subroutine
    LDR R0, R5, #4; R0 = a
    LDR R1, R5, #5; R1 = b
    LD R2, RETMULT; R2 = ret

    WMULT   ADD R1, R1, #0; while (b > 0)
            BRnz  ENDWMULT
            ADD R2, R2, R0; ret += a
            ADD R1, R1, #-1; b--
            BR  WMULT
    ENDWMULT
            STR R2, R5, #3; return ret

    ;Stack Teardown
    LDR R4, R5, #-5; Restore the old value of R4
    LDR R3, R5, #-4; Restore the old value of R3
    LDR R2, R5, #-3; Restore the old value of R2
    LDR R1, R5, #-2; Restore the old value of R1
    LDR R0, R5, #-1; Restore the old value of R0
    ADD R6, R5, #0;  Pop Saved Registers off the Stack and Restore The Stack Pointer
    LDR R5, R6, #1;  Restore The Frame Pointer
    LDR R7, R6, #2;  Restore The Return Address
    ADD R6, R6, #3;  Pop The Return Address, Frame Pointer, and Local Variables off the Stack
    RET

    RETMULT  .fill 0; ret = 0

;;  FACTORIAL Pseudocode (see PDF for explanation and examples)
;;
;;  FACTORIAL(int n) {
;;      int ret = 1;
;;      for (int x = 2; x <= n; x++) {
;;          ret = MULTIPLY(ret, x);
;;      }
;;      return ret;
;;  }

FACTORIAL ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the FACTORIAL subroutine here!

    ;Stack Build Up
    ADD R6, R6, #-4; Push the Return Value, Return Address, Old Frame Pointer, and Local Variables onto the Stack
    STR R7, R6, #2; Save The Return Address to the Stack
    STR R5, R6, #1; Save The Old Frame Pointer to the Stack
    ADD R5, R6, #0; Make the Current Frame Pointer point to the First Local Variable
    ADD R6, R6, #-5; Push The Five General Purpose Registers Onto The Stack
    STR R0, R5, #-1; Save old value of R0 to the Stack
    STR R1, R5, #-2; Save old value of R1 to the Stack
    STR R2, R5, #-3; Save old value of R2 to the Stack
    STR R3, R5, #-4; Save old value of R3 to the Stack
    STR R4, R5, #-5; Save old value of R4 to the Stack

    ;Work of the Subroutine
    LDR R0, R5, #4; R0 = n
    LD R1, RETFACT; R1 = ret
    LD R2, X; R2 = x

    NOT R0, R0; R0 = ~n
    ADD R0, R0, #1; R0 = -n

    WFACT   ADD R3, R2, R0; while (x <= n)
            BRp  ENDWFACT
            ADD R6, R6, #-2; Push Two Arguments onto the Stack
            ;Push Arguments in Reverse order
            STR R2, R6, #1; Push x onto the stack
            STR R1, R6, #0; Push ret onto the stack
            JSR	MULTIPLY; Call the Multiply Subroutine
            LDR R1, R6, #0; ret = MULTIPLY(ret, x)
            ADD R6, R6, #3; Pop the Return Value and the 2 Arguments off the stack
            ADD R2, R2, #1; x++
            BR  WFACT
    ENDWFACT
            STR R1, R5, #3; return ret

    ;Stack Teardown
    LDR R4, R5, #-5; Restore the old value of R4
    LDR R3, R5, #-4; Restore the old value of R3
    LDR R2, R5, #-3; Restore the old value of R2
    LDR R1, R5, #-2; Restore the old value of R1
    LDR R0, R5, #-1; Restore the old value of R0
    ADD R6, R5, #0;  Pop Saved Registers off the Stack and Restore The Stack Pointer
    LDR R5, R6, #1;  Restore The Frame Pointer
    LDR R7, R6, #2;  Restore The Return Address
    ADD R6, R6, #3;  Pop The Return Address, Frame Pointer, and Local Variables off the Stack
    RET

    RETFACT  .fill 1; ret = 1
    X  .fill 2; x = 2

;; Needed to Simulate Subroutine Call in Complx
STACK .fill xF000
.end